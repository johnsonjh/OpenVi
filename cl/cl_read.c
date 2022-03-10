/*      $OpenBSD: cl_read.c,v 1.23 2021/09/02 11:19:02 schwarze Exp $   */

/*-
 * Copyright (c) 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include "../include/compat.h"

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <bitstring.h>
#include <curses.h>
#include <errno.h>
#include <bsd_fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <bsd_termios.h>
#if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ)
# include <termio.h>
#endif /* if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ) */
#include <bsd_unistd.h>

#include "../common/common.h"
#include "../ex/script.h"
#include "cl.h"

#undef open

#if defined(_AIX) || defined(__illumos__)
# undef lines
# undef columns
#endif /* if defined(_AIX) || defined(__illumos__) */

static input_t  cl_read(SCR *,
                    u_int32_t, CHAR_T *, size_t, int *, struct timeval *);
static int      cl_resize(SCR *sp, size_t lines, size_t columns);

/*
 * cl_event --
 *      Return a single event.
 *
 * PUBLIC: int cl_event(SCR *, EVENT *, u_int32_t, int);
 */
int
cl_event(SCR *sp, EVENT *evp, u_int32_t flags, int ms)
{
        struct timeval t, *tp;
        CL_PRIVATE *clp;
        size_t lines, columns;
        int changed, nr;

        /*
         * Queue signal based events.  We never clear SIGHUP or SIGTERM events,
         * so that we just keep returning them until the editor dies.
         */
        clp = CLP(sp);
retest: if (LF_ISSET(EC_INTERRUPT) || cl_sigint) {
                if (cl_sigint) {
                        cl_sigint = 0;
                        evp->e_event = E_INTERRUPT;
                } else
                        evp->e_event = E_TIMEOUT;
                return (0);
        }
        switch (cl_sigterm) {
        case SIGHUP:
                evp->e_event = E_SIGHUP;
                return (0);
        case SIGTERM:
                evp->e_event = E_SIGTERM;
                return (0);
        default:
                break;
        }
        if (cl_sigwinch) {
                cl_sigwinch = 0;
                if (cl_ssize(sp, 1, &lines, &columns, &changed))
                        return (1);
                if (changed) {
                        (void)cl_resize(sp, lines, columns);
                        evp->e_event = E_WRESIZE;
                        return (0);
                }
                /* No real change, ignore the signal. */
        }

        /* Set timer. */
        if (ms == 0)
                tp = NULL;
        else {
                t.tv_sec = ms / 1000;
                t.tv_usec = (ms % 1000) * 1000;
                tp = &t;
        }

        /* Read input characters. */
        switch (cl_read(sp, LF_ISSET(EC_QUOTED | EC_RAW),
            clp->ibuf, sizeof(clp->ibuf), &nr, tp)) {
        case INP_OK:
                evp->e_csp = clp->ibuf;
                evp->e_len = nr;
                evp->e_event = E_STRING;
                break;
        case INP_EOF:
                evp->e_event = E_EOF;
                break;
        case INP_ERR:
                evp->e_event = E_ERR;
                break;
        case INP_INTR:
                goto retest;
        case INP_TIMEOUT:
                evp->e_event = E_TIMEOUT;
                break;
        default:
                abort();
        }
        return (0);
}

/*
 * cl_read --
 *      Read characters from the input.
 */
static input_t
cl_read(SCR *sp, u_int32_t flags, CHAR_T *bp, size_t blen, int *nrp,
    struct timeval *tp)
{
        struct termios term1, term2;
        CL_PRIVATE *clp;
        GS *gp;
        struct pollfd pfd[1];
        input_t rval;
        int nr, term_reset, timeout;

        gp = sp->gp;
        clp = CLP(sp);
        term_reset = 0;

        /*
         * 1: A read from a file or a pipe.  In this case, the reads
         *    never timeout regardless.  This means that we can hang
         *    when trying to complete a map, but we're going to hang
         *    on the next read anyway.
         */
        if (!F_ISSET(clp, CL_STDIN_TTY)) {
                switch (nr = read(STDIN_FILENO, bp, blen)) {
                case 0:
                        return (INP_EOF);
                case -1:
                        goto err;
                default:
                        *nrp = nr;
                        return (INP_OK);
                }
                /* NOTREACHED */
        }

        /*
         * 2: A read with an associated timeout, e.g., trying to complete
         *    a map sequence.  If input exists, we fall into #3.
         */
tty_retry:
        if (tp != NULL) {
                pfd[0].fd = STDIN_FILENO;
                pfd[0].events = POLLIN;
                timeout = tp ? (tp->tv_sec * 1000) + (tp->tv_usec / 1000) : 0;
                switch (poll(pfd, 1, timeout)) {
                case 0:
                        return (INP_TIMEOUT);
                case -1:
                        goto err;
                default:
                        break;
                }
        }

        /*
         * The user can enter a key in the editor to quote a character.  If we
         * get here and the next key is supposed to be quoted, do what we can.
         * Reset the tty so that the user can enter a ^C, ^Q, ^S.  There's an
         * obvious race here, when the key has already been entered, but there's
         * nothing that we can do to fix that problem.
         *
         * The editor can ask for the next literal character even thought it's
         * generally running in line-at-a-time mode.  Do what we can.
         */
        if (LF_ISSET(EC_QUOTED | EC_RAW) && !tcgetattr(STDIN_FILENO, &term1)) {
                term_reset = 1;
                if (LF_ISSET(EC_QUOTED)) {
                        term2 = term1;
                        term2.c_lflag &= ~ISIG;
                        term2.c_iflag &= ~(IXON | IXOFF);
                        (void)tcsetattr(STDIN_FILENO,
                            TCSASOFT | TCSADRAIN, &term2);
                } else
                        (void)tcsetattr(STDIN_FILENO,
                            TCSASOFT | TCSADRAIN, &clp->vi_enter);
        }

        /*
         * 3: Wait for input.
         *
         * Select on the command input and scripting window file descriptors.
         * It's ugly that we wait on scripting file descriptors here, but it's
         * the only way to keep from locking out scripting windows.
         */
        if (F_ISSET(gp, G_SCRWIN)) {
                if (sscr_check_input(sp))
                        goto err;
        }

        /*
         * 4: Read the input.
         *
         * !!!
         * What's going on here is some scary stuff.  Ex runs the terminal in
         * canonical mode.  So, the <newline> character terminating a line of
         * input is returned in the buffer, but a trailing <EOF> character is
         * not similarly included.  As ex uses 0<EOF> and ^<EOF> as autoindent
         * commands, it has to see the trailing <EOF> characters to determine
         * the difference between the user entering "0ab" and "0<EOF>ab".  We
         * leave an extra slot in the buffer, so that we can add a trailing
         * <EOF> character if the buffer isn't terminated by a <newline>.  We
         * lose if the buffer is too small for the line and exactly N characters
         * are entered followed by an <EOF> character.
         */
#define ONE_FOR_EOF     1
        switch (nr = read(STDIN_FILENO, bp, blen - ONE_FOR_EOF)) {
        case  0:                                /* EOF. */
                /*
                 * ^D in canonical mode returns a read of 0, i.e. EOF.  EOF is
                 * a valid command, but we don't want to loop forever because
                 * the terminal driver is returning EOF because the user has
                 * disconnected. The editor will almost certainly try to write
                 * something before this fires, which should kill us, but You
                 * Never Know.
                 */
                if (++clp->eof_count < 50) {
                        bp[0] = clp->orig.c_cc[VEOF];
                        *nrp = 1;
                        rval = INP_OK;

                } else
                        rval = INP_EOF;
                break;
        case -1:                                /* Error or interrupt. */
err:            if (errno == EINTR)
                        rval = INP_INTR;
                else if (errno == EAGAIN)
                        goto tty_retry;
                else {
                        rval = INP_ERR;
                        msgq(sp, M_SYSERR, "input");
                }
                break;
        default:                                /* Input characters. */
                if (F_ISSET(sp, SC_EX) && bp[nr - 1] != '\n')
                        bp[nr++] = clp->orig.c_cc[VEOF];
                *nrp = nr;
                clp->eof_count = 0;
                rval = INP_OK;
                break;
        }

        /* Restore the terminal state if it was modified. */
        if (term_reset)
                (void)tcsetattr(STDIN_FILENO, TCSASOFT | TCSADRAIN, &term1);
        return (rval);
}

/*
 * cl_resize --
 *      Reset the options for a resize event.
 */
static int
cl_resize(SCR *sp, size_t lines, size_t columns)
{
        ARGS *argv[2], a, b;
        char b1[1024];

        a.bp = b1;
        b.bp = NULL;
        a.len = b.len = 0;
        argv[0] = &a;
        argv[1] = &b;

        (void)snprintf(b1, sizeof(b1), "lines=%lu", (u_long)lines);
        a.len = strlen(b1);
        if (opts_set(sp, argv, NULL))
                return (1);
        (void)snprintf(b1, sizeof(b1), "columns=%lu", (u_long)columns);
        a.len = strlen(b1);
        if (opts_set(sp, argv, NULL))
                return (1);
        return (0);
}
