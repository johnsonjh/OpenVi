/*      $OpenBSD: ex_script.c,v 1.27 2017/04/18 01:45:35 deraadt Exp $  */

/*-
 * Copyright (c) 1992, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1992, 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * This code is derived from software contributed to Berkeley by
 * Brian Hirt.
 *
 * See the LICENSE.md file for redistribution information.
 */

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <bitstring.h>
#include <errno.h>
#include <bsd_fcntl.h>
#include <stdio.h>
#include <grp.h>
#include <limits.h>
#include <poll.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>

#if defined(__OpenBSD__) || defined(__NetBSD__)
# include <unistd.h>
# include <termios.h>
# include <util.h>
#else
# include <bsd_termios.h>
# if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ)
#  include <termio.h>
# endif /* if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ) */
# include <bsd_unistd.h>
# include <util.h>
#endif /* if defined(__OpenBSD__) || defined(__NetBSD__) */

#ifndef __FreeBSD__
# ifdef _AIX
#  include <sys/pty.h>
# else
#  ifndef __OpenBSD__
#   if ( !defined(__APPLE__)  && !defined(__MACH__) && \
         !defined(__NetBSD__) )
#    ifndef __illumos__
#     include <pty.h>
#    endif /* ifndef __illumos__ */
#   endif /* if ( !defined(__APPLE__)  && !defined(__MACH__) && \
                  !defined(__NetBSD__) ) */
#  endif /* ifndef __OpenBSD__ */
# endif /* ifdef _AIX */
#else
# include <libutil.h>
#endif /* ifndef __FreeBSD__ */

#include "../common/common.h"
#include "../vi/vi.h"
#include "script.h"
#include "pathnames.h"

#if defined(__OpenBSD__) || defined(__NetBSD__) || \
  ( defined(__APPLE__) && defined(__MACH__) )
int openpty(int *, int *, char *, struct termios *, struct winsize *);
#endif /* defined(__OpenBSD__) || defined(__NetBSD__) ||
        ( defined(__APPLE__) && defined(__MACH__) ) */

#if defined(_AIX) || defined(__illumos__)
# define HAVE_SYS5_PTY
#endif /* if defined(_AIX) || defined(__illumos__) */

#ifdef HAVE_SYS5_PTY
# include <sys/stropts.h>
static int ptys_open(int fdm, char *pts_name);
static int ptym_open(char *pts_name);
static int sscr_pty(int *amaster, int *aslave, char *name,
                    struct termios *termp, void *winp);
#endif /* ifdef HAVE_SYS5_PTY */

static void     sscr_check(SCR *);
static int      sscr_getprompt(SCR *);
static int      sscr_init(SCR *);
static int      sscr_insert(SCR *);
static int      sscr_matchprompt(SCR *, char *, size_t, size_t *);
static int      sscr_setprompt(SCR *, char *, size_t);

/*
 * ex_script -- : sc[ript][!] [file]
 *      Switch to script mode.
 *
 * PUBLIC: int ex_script(SCR *, EXCMD *);
 */
int
ex_script(SCR *sp, EXCMD *cmdp)
{
        /* Vi only command. */
        if (!F_ISSET(sp, SC_VI)) {
                msgq(sp, M_ERR,
                    "The script command is only available in vi mode");
                return (1);
        }

        /* Switch to the new file. */
        if (cmdp->argc != 0 && ex_edit(sp, cmdp))
                return (1);

        /* Create the shell, figure out the prompt. */
        if (sscr_init(sp))
                return (1);

        return (0);
}

/*
 * sscr_init --
 *      Create a pty setup for a shell.
 */
static int
sscr_init(SCR *sp)
{
        SCRIPT *sc;
        char *sh, *sh_path;

        /* We're going to need a shell. */
        if (opts_empty(sp, O_SHELL, 0))
                return (1);

        MALLOC_RET(sp, sc, sizeof(SCRIPT));
        sp->script = sc;
        sc->sh_prompt = NULL;
        sc->sh_prompt_len = 0;

        /*
         * There are two different processes running through this code.
         * They are the shell and the parent.
         */
        sc->sh_master = sc->sh_slave = -1;

        if (tcgetattr(STDIN_FILENO, &sc->sh_term) == -1) {
                msgq(sp, M_SYSERR, "tcgetattr");
                goto err;
        }

        /*
         * Turn off output postprocessing and echo.
         */
        sc->sh_term.c_oflag &= ~OPOST;
        sc->sh_term.c_cflag &= ~(ECHO|ECHOE|ECHONL|ECHOK);

        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &sc->sh_win) == -1) {
                msgq(sp, M_SYSERR, "tcgetattr");
                goto err;
        }

#ifdef HAVE_SYS5_PTY
# ifdef TIOCGWINSZ
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &sc->sh_win) == -1) {
                msgq(sp, M_SYSERR, "tcgetattr");
                goto err;
        }

        if (sscr_pty(&sc->sh_master,
                &sc->sh_slave, sc->sh_name, &sc->sh_term, &sc->sh_win) == -1) {
                msgq(sp, M_SYSERR, "pty");
                goto err;
        }
# else
        if (sscr_pty(&sc->sh_master,
                &sc->sh_slave, sc->sh_name, &sc->sh_term, NULL) == -1) {
                msgq(sp, M_SYSERR, "pty");
                goto err;
        }
# endif /* ifdef TIOCGWINSZ */
#else
        if (openpty(&sc->sh_master,
                &sc->sh_slave, sc->sh_name, &sc->sh_term, &sc->sh_win) == -1) {
                    msgq(sp, M_SYSERR, "pty");
                    goto err;
        }
#endif /* HAVE_SYS5_PTY */

        switch (sc->sh_pid = fork()) {
        case -1:                        /* Error. */
                msgq(sp, M_SYSERR, "fork");
err:            if (sc->sh_master != -1)
                        (void)close(sc->sh_master);
                if (sc->sh_slave != -1)
                        (void)close(sc->sh_slave);
                return (1);
        case 0:                         /* Utility. */
                /*
                 * XXX
                 * So that shells that do command line editing turn it off.
                 */
                if (setenv("TERM", "emacs", 1) == -1 ||
                    setenv("TERMCAP", "emacs:", 1) == -1 ||
                    setenv("EMACS", "t", 1) == -1)
                        _exit(126);

                (void)setsid();
#ifdef TIOCSCTTY
                /*
                 * 4.4BSD allocates a controlling terminal using the TIOCSCTTY
                 * ioctl, not by opening a terminal device file.  POSIX 1003.1
                 * doesn't define a portable way to do this.
                 */
                (void)ioctl(sc->sh_slave, TIOCSCTTY, 0);
#endif /* ifdef TIOCSCTTY */
                (void)close(sc->sh_master);
                (void)dup2(sc->sh_slave, STDIN_FILENO);
                (void)dup2(sc->sh_slave, STDOUT_FILENO);
                (void)dup2(sc->sh_slave, STDERR_FILENO);
                (void)close(sc->sh_slave);

                /* Assumes that all shells have -i. */
                sh_path = O_STR(sp, O_SHELL);
                if ((sh = strrchr(sh_path, '/')) == NULL)
                        sh = sh_path;
                else
                        ++sh;
                execl(sh_path, sh, "-i", (char *)NULL);
                msgq_str(sp, M_SYSERR, sh_path, "execl: %s");
                _exit(127);
        default:                        /* Parent. */
                break;
        }

        if (sscr_getprompt(sp))
                return (1);

        F_SET(sp, SC_SCRIPT);
        F_SET(sp->gp, G_SCRWIN);
        return (0);
}

/*
 * sscr_getprompt --
 *      Eat lines printed by the shell until a line with no trailing
 *      carriage return comes; set the prompt from that line.
 */
static int
sscr_getprompt(SCR *sp)
{
        CHAR_T *endp, *p, *t, buf[1024];
        SCRIPT *sc;
        struct pollfd pfd[1];
        recno_t lline;
        size_t llen, len;
        u_int value;
        int nr;

        endp = buf;
        len = sizeof(buf);
        (void)len;

        /* Wait up to a second for characters to read. */
        sc = sp->script;
        pfd[0].fd = sc->sh_master;
        pfd[0].events = POLLIN;
        switch (poll(pfd, 1, 5 * 1000)) {
        case -1:                /* Error or interrupt. */
                msgq(sp, M_SYSERR, "poll");
                goto prompterr;
        case  0:                /* Timeout */
                msgq(sp, M_ERR, "Error: timed out");
                goto prompterr;
        default:                /* Characters to read. */
                break;
        }

        /* Read the characters. */
more:   len = sizeof(buf) - (endp - buf);
        switch (nr = read(sc->sh_master, endp, len)) {
        case  0:                        /* EOF. */
                msgq(sp, M_ERR, "Error: shell: EOF");
                goto prompterr;
        case -1:                        /* Error or interrupt. */
                msgq(sp, M_SYSERR, "shell");
                goto prompterr;
        default:
                endp += nr;
                break;
        }

        /* If any complete lines, push them into the file. */
        for (p = t = buf; p < endp; ++p) {
                value = KEY_VAL(sp, *p);
                if (value == K_CR || value == K_NL) {
                        if (db_last(sp, &lline) ||
                            db_append(sp, 0, lline, t, p - t))
                                goto prompterr;
                        t = p + 1;
                }
        }
        if (p > buf) {
                memmove(buf, t, endp - t);
                endp = buf + (endp - t);
        }
        if (endp == buf)
                goto more;

        /* Wait up 1/10 of a second to make sure that we got it all. */
        switch (poll(pfd, 1, 100)) {
        case -1:                /* Error or interrupt. */
                msgq(sp, M_SYSERR, "poll");
                goto prompterr;
        case  0:                /* Timeout */
                break;
        default:                /* Characters to read. */
                goto more;
        }

        /* Timed out, so theoretically we have a prompt. */
        llen = endp - buf;
        (void)llen;
        endp = buf;
        (void)endp;

        /* Append the line into the file. */
        if (db_last(sp, &lline) || db_append(sp, 0, lline, buf, llen)) {
prompterr:      sscr_end(sp);
                return (1);
        }

        return (sscr_setprompt(sp, buf, llen));
}

/*
 * sscr_exec --
 *      Take a line and hand it off to the shell.
 *
 * PUBLIC: int sscr_exec(SCR *, recno_t);
 */
int
sscr_exec(SCR *sp, recno_t lno)
{
        SCRIPT *sc;
        recno_t last_lno;
        size_t blen, len, last_len, tlen;
        int isempty, matchprompt, nw, rval;
        char *bp, *p;

        /* If there's a prompt on the last line, append the command. */
        if (sp == NULL)
                return (1);
        if (db_last(sp, &last_lno))
                return (1);
        if (db_get(sp, last_lno, DBG_FATAL, &p, &last_len))
                return (1);
        if (sscr_matchprompt(sp, p, last_len, &tlen) && tlen == 0) {
                matchprompt = 1;
                GET_SPACE_RET(sp, bp, blen, last_len + 128);
                if (bp != NULL)
                    memmove(bp, p, last_len);
        } else
                matchprompt = 0;

        /* Get something to execute. */
        if (db_eget(sp, lno, &p, &len, &isempty)) {
                if (isempty)
                        goto empty;
                goto err1;
        }

        /* Empty lines aren't interesting. */
        if (len == 0)
                goto empty;

        /* Delete any prompt. */
        if (sscr_matchprompt(sp, p, len, &tlen)) {
                if (tlen == len) {
empty:                  msgq(sp, M_BERR, "No command to execute");
                        goto err1;
                }
                p += (len - tlen);
                len = tlen;
        }

        /* Push the line to the shell. */
        sc = sp->script;
        if ((nw = write(sc->sh_master, p, len)) != len)
                goto err2;
        rval = 0;
        if (write(sc->sh_master, "\n", 1) != 1) {
err2:           if (nw == 0)
                        errno = EIO;
                msgq(sp, M_SYSERR, "shell");
                goto err1;
        }

        if (matchprompt) {
                ADD_SPACE_RET(sp, bp, blen, last_len + len);
                if (bp != NULL) {
                    memmove(bp + last_len, p, len);
                    if (db_set(sp, last_lno, bp, last_len + len))
err1:                       rval = 1;
                }
        }
        if (matchprompt)
                FREE_SPACE(sp, bp, blen);
        return (rval);
}

/*
 * sscr_check_input -
 *      Check for input from command input or scripting windows.
 *
 * PUBLIC: int sscr_check_input(SCR *sp);
 */
int
sscr_check_input(SCR *sp)
{
        GS *gp;
        SCR *tsp;
        struct pollfd *pfd;
        int nfds, rval;

        gp = sp->gp;
        rval = 0;

        /* Allocate space for pfd. */
        nfds = 1;
        TAILQ_FOREACH(tsp, &gp->dq, q)
                if (F_ISSET(sp, SC_SCRIPT))
                        nfds++;
        pfd = calloc(nfds, sizeof(struct pollfd));
        if (pfd == NULL) {
                msgq(sp, M_SYSERR, "malloc");
                return (1);
        }

        /* Setup events bitmasks. */
        pfd[0].fd = STDIN_FILENO;
        pfd[0].events = POLLIN;
        nfds = 1;
        TAILQ_FOREACH(tsp, &gp->dq, q)
                if (F_ISSET(sp, SC_SCRIPT)) {
                        pfd[nfds].fd = sp->script->sh_master;
                        pfd[nfds].events = POLLIN;
                        nfds++;
                }

loop:
        /* Check for input. */
        switch (poll(pfd, nfds, INFTIM)) {
        case -1:
                msgq(sp, M_SYSERR, "poll");
                rval = 1;
                /* FALLTHROUGH */
        case 0:
                goto done;
        default:
                break;
        }

        /* Only insert from the scripting windows if no command input */
        if (!(pfd[0].revents & POLLIN)) {
                nfds = 1;
                TAILQ_FOREACH(tsp, &gp->dq, q)
                        if (F_ISSET(sp, SC_SCRIPT)) {
                                if ((pfd[nfds].revents & POLLHUP) && sscr_end(sp))
                                        goto done;
                                if ((pfd[nfds].revents & POLLIN) && sscr_insert(sp))
                                        goto done;
                                nfds++;
                        }
                goto loop;
        }
done:
        free(pfd);
        return (rval);
}

/*
 * sscr_input --
 *      Read any waiting shell input.
 *
 * PUBLIC: int sscr_input(SCR *);
 */
int
sscr_input(SCR *sp)
{
        GS *gp;
        struct pollfd *pfd;
        int nfds, rval;

        gp = sp->gp;
        rval = 0;

        /* Allocate space for pfd. */
        nfds = 0;
        TAILQ_FOREACH(sp, &gp->dq, q)
                if (F_ISSET(sp, SC_SCRIPT))
                        nfds++;
        if (nfds == 0)
                return (0);
        pfd = calloc(nfds, sizeof(struct pollfd));
        if (pfd == NULL) {
                msgq(sp, M_SYSERR, "malloc");
                return (1);
        }

        /* Setup events bitmasks. */
        nfds = 0;
        TAILQ_FOREACH(sp, &gp->dq, q)
                if (F_ISSET(sp, SC_SCRIPT)) {
                        pfd[nfds].fd = sp->script->sh_master;
                        pfd[nfds].events = POLLIN;
                        nfds++;
                }

loop:
        /* Check for input. */
        switch (poll(pfd, nfds, 0)) {
        case -1:
                msgq(sp, M_SYSERR, "poll");
                rval = 1;
                /* FALLTHROUGH */
        case 0:
                goto done;
        default:
                break;
        }

        /* Read the input. */
        nfds = 0;
        TAILQ_FOREACH(sp, &gp->dq, q)
                if (F_ISSET(sp, SC_SCRIPT)) {
                        if ((pfd[nfds].revents & POLLHUP) && sscr_end(sp))
                                goto done;
                        if ((pfd[nfds].revents & POLLIN) && sscr_insert(sp))
                                goto done;
                        nfds++;
                }
        goto loop;
done:
        free(pfd);
        return (rval);
}

/*
 * sscr_insert --
 *      Take a line from the shell and insert it into the file.
 */
static int
sscr_insert(SCR *sp)
{
        CHAR_T *endp, *p, *t;
        SCRIPT *sc;
        struct pollfd pfd[1];
        recno_t lno;
        size_t blen, len, tlen;
        u_int value;
        int nr, rval;
        char *bp;

        /* Find out where the end of the file is. */
        if (db_last(sp, &lno))
                return (1);

#define MINREAD 1024
        GET_SPACE_RET(sp, bp, blen, MINREAD);
        endp = bp;

        /* Read the characters. */
        rval = 1;
        sc = sp->script;
more:   switch (nr = read(sc->sh_master, endp, MINREAD)) {
        case  0:                        /* EOF; shell just exited. */
                sscr_end(sp);
                rval = 0;
                goto ret;
        case -1:                        /* Error or interrupt. */
                msgq(sp, M_SYSERR, "shell");
                goto ret;
        default:
                endp += nr;
                break;
        }

        /* Append the lines into the file. */
        for (p = t = bp; p < endp; ++p) {
                value = KEY_VAL(sp, *p);
                if (value == K_CR || value == K_NL) {
                        len = p - t;
                        if (db_append(sp, 1, lno++, t, len))
                                goto ret;
                        t = p + 1;
                }
        }
        if (p > t) {
                len = p - t;
                /*
                 * If the last thing from the shell isn't another prompt, wait
                 * up to 1/10 of a second for more stuff to show up, so that
                 * we don't break the output into two separate lines.  Don't
                 * want to hang indefinitely because some program is hanging,
                 * confused the shell, or whatever.
                 */
                if (!sscr_matchprompt(sp, t, len, &tlen) || tlen != 0) {
                        pfd[0].fd = sc->sh_master;
                        pfd[0].events = POLLIN;
                        if (poll(pfd, 1, 100) > 0) {
                                memmove(bp, t, len);
                                endp = bp + len;
                                goto more;
                        }
                }
                if (sscr_setprompt(sp, t, len))
                        return (1);
                if (db_append(sp, 1, lno++, t, len))
                        goto ret;
        }

        /* The cursor moves to EOF. */
        sp->lno = lno;
        sp->cno = len ? len - 1 : 0;
        rval = vs_refresh(sp, 1);

ret:    FREE_SPACE(sp, bp, blen);
        return (rval);
}

/*
 * sscr_setprompt --
 *
 * Set the prompt to the last line we got from the shell.
 *
 */
static int
sscr_setprompt(SCR *sp, char *buf, size_t len)
{
        SCRIPT *sc;

        sc = sp->script;
        free(sc->sh_prompt);
        MALLOC(sp, sc->sh_prompt, len + 1);
        if (sc->sh_prompt == NULL) {
                sscr_end(sp);
                return (1);
        }
        memmove(sc->sh_prompt, buf, len);
        sc->sh_prompt_len = len;
        sc->sh_prompt[len] = '\0';
        return (0);
}

/*
 * sscr_matchprompt --
 *      Check to see if a line matches the prompt.  Nul's indicate
 *      parts that can change, in both content and size.
 */
static int
sscr_matchprompt(SCR *sp, char *lp, size_t line_len, size_t *lenp)
{
        SCRIPT *sc;
        size_t prompt_len;
        char *pp;

        sc = sp->script;
        if (line_len < (prompt_len = sc->sh_prompt_len))
                return (0);

        for (pp = sc->sh_prompt;
            prompt_len && line_len; --prompt_len, --line_len) {
                if (*pp == '\0') {
                        for (; prompt_len && *pp == '\0'; --prompt_len, ++pp);
                        if (!prompt_len)
                                return (0);
                        for (; line_len && *lp != *pp; --line_len, ++lp);
                        if (!line_len)
                                return (0);
                }
                if (*pp++ != *lp++)
                        break;
        }

        if (prompt_len)
                return (0);
        if (lenp != NULL)
                *lenp = line_len;
        return (1);
}

/*
 * sscr_end --
 *      End the pipe to a shell.
 *
 * PUBLIC: int sscr_end(SCR *);
 */
int
sscr_end(SCR *sp)
{
        SCRIPT *sc;

        if ((sc = sp->script) == NULL)
                return (0);

        /* Turn off the script flags. */
        F_CLR(sp, SC_SCRIPT);
        sscr_check(sp);

        /* Close down the parent's file descriptors. */
        if (sc->sh_master != -1)
            (void)close(sc->sh_master);
        if (sc->sh_slave != -1)
            (void)close(sc->sh_slave);

        /* This should have killed the child. */
        (void)proc_wait(sp, sc->sh_pid, "script-shell", 0, 0);

        /* Free memory. */
        free(sc->sh_prompt);
        free(sc);
        sp->script = NULL;

        return (0);
}

/*
 * sscr_check --
 *      Set/clear the global scripting bit.
 */
static void
sscr_check(SCR *sp)
{
        GS *gp;

        gp = sp->gp;
        TAILQ_FOREACH(sp, &gp->dq, q)
                if (F_ISSET(sp, SC_SCRIPT)) {
                        F_SET(gp, G_SCRWIN);
                        return;
                }
        F_CLR(gp, G_SCRWIN);
}

#ifdef HAVE_SYS5_PTY
static int
sscr_pty(int *amaster, int *aslave, char *name,
         struct termios *termp, void *winp)
{
    int master, slave;

    /* open master terminal */
    if ((master = ptym_open(name)) < 0)  {
        errno = ENOENT;        /* out of ptys */
        return (-1);
    }

    /* open slave terminal */
    if ((slave = ptys_open(master, name)) >= 0) {
        *amaster = master;
        *aslave = slave;
    } else {
        errno = ENOENT;        /* out of ptys */
        return (-1);
    }

    if (termp)
        (void) tcsetattr(slave, TCSAFLUSH, termp);
# ifdef TIOCSWINSZ
    if (winp != NULL)
        (void) ioctl(slave, TIOCSWINSZ, (struct winsize *)winp);
# endif /* ifdef TIOCSWINSZ */
    return (0);
}

/*
 * ptym_open --
 *    This function opens a master pty and returns the file descriptor
 *    to it.  pts_name is also returned which is the name of the slave.
 */
static int
ptym_open(char *pts_name)
{
    int fdm;
    char *ptr, *ptsname();

    strcpy(pts_name, _PATH_SYSV_PTY);
    if ((fdm = open(pts_name, O_RDWR)) < 0 )
        return (-1);

    if (grantpt(fdm) < 0) {
        close(fdm);
        return (-2);
    }

    if (unlockpt(fdm) < 0) {
        close(fdm);
        return (-3);
    }

    if (unlockpt(fdm) < 0) {
        close(fdm);
        return (-3);
    }

    /* get slave's name */
    if ((ptr = ptsname(fdm)) == NULL) {
        close(fdm);
        return (-3);
    }
    strcpy(pts_name, ptr);
    return (fdm);
}

/*
 * ptys_open --
 *     This function opens the slave pty.
 */
static int
ptys_open(int fdm, char *pts_name)
{
    int fds;

    if ((fds = open(pts_name, O_RDWR)) < 0) {
        close(fdm);
        return (-5);
    }

    if (ioctl(fds, I_PUSH, "ptem") < 0) {
        close(fds);
        close(fdm);
        return (-6);
    }

    if (ioctl(fds, I_PUSH, "ldterm") < 0) {
        close(fds);
        close(fdm);
        return (-7);
    }

    if (ioctl(fds, I_PUSH, "ttcompat") < 0) {
        close(fds);
        close(fdm);
        return (-8);
    }

    return (fds);
}
#endif /* HAVE_SYS5_PTY */
