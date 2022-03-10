/*      $OpenBSD: cl_main.c,v 1.36 2021/10/24 21:24:17 deraadt Exp $    */

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

#include <bitstring.h>
#include <curses.h>
#include <bsd_err.h>
#include <errno.h>
#include <bsd_fcntl.h>
#include <paths.h>
#ifdef __solaris__
# define _XPG7
# undef __EXTENSIONS__
# define __EXTENSIONS__
# include <sys/procset.h>
#endif /* ifdef __solaris__ */
#include <signal.h>
#include <stdio.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <term.h>
#include <bsd_termios.h>
#if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ)
# include <termio.h>
#endif /* #if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ) */
#include <bsd_unistd.h>

#include "errc.h"

#ifdef __solaris__
# undef GS
#endif /* ifdef __solaris__ */
#include "../common/common.h"
#include "cl.h"

#undef open

void cl_imctrl (SCR *, imctrl_t);

GS *__global_list;                              /* GLOBAL: List of screens. */

volatile sig_atomic_t cl_sigint;
volatile sig_atomic_t cl_sigterm;
volatile sig_atomic_t cl_sigwinch;

static void        cl_func_std(GS *);
static CL_PRIVATE *cl_init(GS *);
static GS         *gs_init(void);
static int         setsig(int, struct sigaction *, void (*)(int));
static void        sig_end(GS *);
static void        term_init(char *);

/*
 * main --
 *      This is the main loop for the standalone curses editor.
 */
int
main(int argc, char *argv[])
{
        CL_PRIVATE *clp;
        GS *gp;
        size_t rows, cols;
        int rval;
        char *ttype;

        /* Create and initialize the global structure. */
        __global_list = gp = gs_init();

        /* Create and initialize the CL_PRIVATE structure. */
        clp = cl_init(gp);

        /*
         * Initialize the terminal information.
         *
         * We have to know what terminal it is from the start, since we may
         * have to use termcap/terminfo to find out how big the screen is.
         */
        ttype = getenv("TERM");
        if (ttype == NULL)
                ttype = "unknown";
        term_init(ttype);
        ttype = getenv("TERM");

        /* Add the terminal type to the global structure. */
        if ((OG_D_STR(gp, GO_TERM) =
            OG_STR(gp, GO_TERM) = strdup(ttype)) == NULL)
                openbsd_err(1, NULL);

        /* Figure out how big the screen is. */
        if (cl_ssize(NULL, 0, &rows, &cols, NULL))
                exit (1);

        /* Add the rows and columns to the global structure. */
        OG_VAL(gp, GO_LINES) = OG_D_VAL(gp, GO_LINES) = rows;
        OG_VAL(gp, GO_COLUMNS) = OG_D_VAL(gp, GO_COLUMNS) = cols;

        /* Ex wants stdout to be buffered. */
        (void)setvbuf(stdout, NULL, _IOFBF, 0);

        /* Start catching signals. */
        if (sig_init(gp, NULL))
                exit (1);

        /* Run ex/vi. */
        rval = editor(gp, argc, argv);

        /* Clean up signals. */
        sig_end(gp);

        /* Clean up the terminal. */
        (void)cl_quit(gp);

        /*
         * XXX
         * Reset the O_MESG option.
         */
        if (clp->tgw != TGW_UNKNOWN)
                (void)cl_omesg(NULL, clp, clp->tgw == TGW_SET);

        /*
         * XXX
         * Reset the X11 xterm icon/window name.
         */
        if (F_ISSET(clp, CL_RENAME)) {
                (void)printf(XTERM_RENAME, ttype);
                (void)fflush(stdout);
        }

        /* If a killer signal arrived, pretend we just got it. */
        if (cl_sigterm) {
                (void)signal(cl_sigterm, SIG_DFL);
                (void)kill(getpid(), cl_sigterm);
                /* NOTREACHED */
        }

        /* Free the global and CL private areas. */
#if defined(DEBUG) || defined(PURIFY)
        free(clp);
        free(gp);
#endif /* if defined(DEBUG) || defined(PURIFY) */

        exit (rval);
}

/*
 * gs_init --
 *      Create and partially initialize the GS structure.
 */
static GS *
gs_init(void)
{
        GS *gp;

        /* Allocate the global structure. */
        if ((gp = calloc(1, sizeof(GS))) == NULL)
                openbsd_err(1, NULL);

        return (gp);
}

/*
 * cl_init --
 *      Create and partially initialize the CL structure.
 */
static CL_PRIVATE *
cl_init(GS *gp)
{
        CL_PRIVATE *clp;
        int fd;

        /* Allocate the CL private structure. */
        if ((clp = calloc(1, sizeof(CL_PRIVATE))) == NULL)
                openbsd_err(1, NULL);
        gp->cl_private = clp;

        /*
         * Set the CL_STDIN_TTY flag.  It's purpose is to avoid setting
         * and resetting the tty if the input isn't from there.  We also
         * use the same test to determine if we're running a script or
         * not.
         */
        if (isatty(STDIN_FILENO))
                F_SET(clp, CL_STDIN_TTY);
        else
                F_SET(gp, G_SCRIPTED);

        /*
         * We expect that if we've lost our controlling terminal that the
         * open() (but not the tcgetattr()) will fail.
         */
        if (F_ISSET(clp, CL_STDIN_TTY)) {
                if (tcgetattr(STDIN_FILENO, &clp->orig) == -1)
                        goto tcfail;
        } else if ((fd = open(_PATH_TTY, O_RDONLY)) != -1) {
                if (tcgetattr(fd, &clp->orig) == -1)
tcfail:                 openbsd_err(1, "tcgetattr");
                (void)close(fd);
        }

        /* Initialize the list of curses functions. */
        cl_func_std(gp);

        return (clp);
}

/*
 * term_init --
 *      Initialize terminal information.
 */
static void
term_init(char *ttype)
{
        int err;

        /* Set up the terminal database information. */
        setupterm(ttype, STDOUT_FILENO, &err);
        if (err == 0) {
                if (strlen(ttype) == 0)
                        ttype = "unknown";
                (void)fprintf(stderr,
                        "%s: Unknown terminal type '%s'; falling back to 'vt100'\n",
                                bsd_getprogname(), ttype);
                sleep(5);
                setenv("TERM", "vt100", 1);
                setupterm("vt100", STDOUT_FILENO, &err);
        }
        switch (err) {
        case -1:
                openbsd_errx(1, "No terminal database found");
        case 0:
                openbsd_errx(1, "%s: unknown terminal type", ttype);
        }
}

static void
h_int(int signo)
{
        cl_sigint = 1;
}

static void
h_term(int signo)
{
        cl_sigterm = signo;
}

static void
h_winch(int signo)
{
        cl_sigwinch = 1;
}

/*
 * sig_init --
 *      Initialize signals.
 *
 * PUBLIC: int sig_init(GS *, SCR *);
 */
int
sig_init(GS *gp, SCR *sp)
{
        CL_PRIVATE *clp;

        clp = GCLP(gp);

        cl_sigint   = 0;
        cl_sigterm  = 0;
        cl_sigwinch = 0;

        if (sp == NULL) {
                if (setsig(SIGHUP,   &clp->oact[INDX_HUP],   h_term) ||
                    setsig(SIGINT,   &clp->oact[INDX_INT],   h_int)  ||
                    setsig(SIGQUIT,  &clp->oact[INDX_INT],   h_int)  ||
                    setsig(SIGTERM,  &clp->oact[INDX_TERM],  h_term) ||
                    setsig(SIGWINCH, &clp->oact[INDX_WINCH], h_winch)
                    )
                        openbsd_err(1, NULL);
        } else
                if (setsig(SIGHUP,   NULL, h_term) ||
                    setsig(SIGINT,   NULL, h_int)  ||
                    setsig(SIGQUIT,  NULL, h_int)  ||
                    setsig(SIGTERM,  NULL, h_term) ||
                    setsig(SIGWINCH, NULL, h_winch)
                    ) {
                        msgq(sp, M_SYSERR, "signal-reset");
                }
        return (0);
}

/*
 * setsig --
 *      Set a signal handler.
 */
static int
setsig(int signo, struct sigaction *oactp, void (*handler)(int))
{
        struct sigaction act;

        /*
         * Use sigaction(2), not signal(3), since we don't always want to
         * restart system calls.  The example is when waiting for a command
         * mode keystroke and SIGWINCH arrives.  Besides, you can't portably
         * restart system calls (thanks, POSIX!).
         */
        act.sa_handler = handler;
        sigemptyset(&act.sa_mask);

        act.sa_flags = 0;
        return (sigaction(signo, &act, oactp));
}

/*
 * sig_end --
 *      End signal setup.
 */
static void
sig_end(GS *gp)
{
        CL_PRIVATE *clp;

        clp = GCLP(gp);
        (void)sigaction(SIGHUP,   NULL, &clp->oact[INDX_HUP]);
        (void)sigaction(SIGINT,   NULL, &clp->oact[INDX_INT]);
        (void)sigaction(SIGQUIT,  NULL, &clp->oact[INDX_INT]);
        (void)sigaction(SIGTERM,  NULL, &clp->oact[INDX_TERM]);
        (void)sigaction(SIGWINCH, NULL, &clp->oact[INDX_WINCH]);
}

/*
 * cl_func_std --
 *      Initialize the standard curses functions.
 */
static void
cl_func_std(GS *gp)
{
        gp->scr_addstr    = cl_addstr;
        gp->scr_attr      = cl_attr;
        gp->scr_baud      = cl_baud;
        gp->scr_bell      = cl_bell;
        gp->scr_busy      = NULL;
        gp->scr_clrtoeol  = cl_clrtoeol;
        gp->scr_cursor    = cl_cursor;
        gp->scr_deleteln  = cl_deleteln;
        gp->scr_event     = cl_event;
        gp->scr_ex_adjust = cl_ex_adjust;
        gp->scr_fmap      = cl_fmap;
        gp->scr_imctrl    = cl_imctrl;
        gp->scr_insertln  = cl_insertln;
        gp->scr_keyval    = cl_keyval;
        gp->scr_move      = cl_move;
        gp->scr_msg       = NULL;
        gp->scr_optchange = cl_optchange;
        gp->scr_refresh   = cl_refresh;
        gp->scr_rename    = cl_rename;
        gp->scr_screen    = cl_screen;
        gp->scr_suspend   = cl_suspend;
        gp->scr_usage     = cl_usage;
}
