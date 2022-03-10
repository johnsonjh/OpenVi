/*      $OpenBSD: ex_shell.c,v 1.15 2015/03/28 12:54:37 bcallah Exp $   */

/*-
 * Copyright (c) 1992, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1992, 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include <sys/queue.h>
#include <sys/wait.h>

#include <bitstring.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#ifdef __solaris__
# define _XPG7
#endif /* ifdef __solaris__ */
#include <signal.h>
#include <stdio.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <bsd_unistd.h>

#include "../common/common.h"

#define MINIMUM(a, b)   (((a) < (b)) ? (a) : (b))

/*
 * ex_shell -- :sh[ell]
 *      Invoke the program named in the SHELL environment variable
 *      with the argument -i.
 *
 * PUBLIC: int ex_shell(SCR *, EXCMD *);
 */
int
ex_shell(SCR *sp, EXCMD *cmdp)
{
        int rval;
        char buf[PATH_MAX];

        /* We'll need a shell. */
        if (opts_empty(sp, O_SHELL, 0))
                return (1);

        /*
         * XXX
         * Assumes all shells use -i.
         */
        (void)snprintf(buf, sizeof(buf), "%s -i", O_STR(sp, O_SHELL));

        /* Restore the window name. */
        (void)sp->gp->scr_rename(sp, NULL, 0);

        /* If we're still in a vi screen, move out explicitly. */
        rval = ex_exec_proc(sp, cmdp, buf, NULL, !F_ISSET(sp, SC_SCR_EXWROTE));

        /* Set the window name. */
        (void)sp->gp->scr_rename(sp, sp->frp->name, 1);

        /*
         * !!!
         * Historically, vi didn't require a continue message after the
         * return of the shell.  Match it.
         */
        F_SET(sp, SC_EX_WAIT_NO);

        return (rval);
}

/*
 * ex_exec_proc --
 *      Run a separate process.
 *
 * PUBLIC: int ex_exec_proc(SCR *, EXCMD *, char *, const char *, int);
 */
int
ex_exec_proc(SCR *sp, EXCMD *cmdp, char *cmd, const char *msg,
    int need_newline)
{
        GS *gp;
        const char *name;
        pid_t pid;

        gp = sp->gp;

        /* We'll need a shell. */
        if (opts_empty(sp, O_SHELL, 0))
                return (1);

        /* Enter ex mode. */
        if (F_ISSET(sp, SC_VI)) {
                if (gp->scr_screen(sp, SC_EX)) {
                        ex_emsg(sp, cmdp->cmd->name, EXM_NOCANON);
                        return (1);
                }
                (void)gp->scr_attr(sp, SA_ALTERNATE, 0);
                F_SET(sp, SC_SCR_EX | SC_SCR_EXWROTE);
        }

        /* Put out additional newline, message. */
        if (need_newline)
                (void)ex_puts(sp, "\n");
        if (msg != NULL) {
                (void)ex_puts(sp, msg);
                (void)ex_puts(sp, "\n");
        }
        (void)ex_fflush(sp);

        switch (pid = fork()) {
        case -1:                        /* Error. */
                msgq(sp, M_SYSERR, "fork");
                return (1);
        case 0:                         /* Utility. */
                if ((name = strrchr(O_STR(sp, O_SHELL), '/')) == NULL)
                        name = O_STR(sp, O_SHELL);
                else
                        ++name;
                execl(O_STR(sp, O_SHELL), name, "-c", cmd, (char *)NULL);
                msgq_str(sp, M_SYSERR, O_STR(sp, O_SHELL), "execl: %s");
                _exit(127);
                /* NOTREACHED */
        default:                        /* Parent. */
                return (proc_wait(sp, pid, cmd, 0, 0));
        }
        /* NOTREACHED */
}

/*
 * proc_wait --
 *      Wait for one of the processes.
 *
 * !!!
 * The pid_t type varies in size from a short to a long depending on the
 * system.  It has to be cast into something or the standard promotion
 * rules get you.  I'm using a long based on the belief that nobody is
 * going to make it unsigned and it's unlikely to be a quad.
 *
 * PUBLIC: int proc_wait(SCR *, pid_t, const char *, int, int);
 */
int
proc_wait(SCR *sp, pid_t pid, const char *cmd, int silent, int okpipe)
{
        size_t len;
        int nf, pstat;
        char *p;

        /* Wait for the utility, ignoring interruptions. */
        for (;;) {
                errno = 0;
                if (waitpid(pid, &pstat, 0) != -1)
                        break;
                if (errno != EINTR) {
                        msgq(sp, M_SYSERR, "waitpid");
                        return (1);
                }
        }

        /*
         * Display the utility's exit status.  Ignore SIGPIPE from the
         * parent-writer, as that only means that the utility chose to
         * exit before reading all of its input.
         */
        if (WIFSIGNALED(pstat) && (!okpipe || WTERMSIG(pstat) != SIGPIPE)) {
                for (; isblank(*cmd); ++cmd);
                p = msg_print(sp, cmd, &nf);
                len = strlen(p);
                msgq(sp, M_ERR, "%.*s%s: received signal: %s%s",
                     MINIMUM(len, 20),
                     p,
                     len > 20 ? " ..." : "",
                     strsignal(WTERMSIG(pstat)),
#ifdef WCOREDUMP
                     WCOREDUMP(pstat) ? "; core dumped" : "");
#else
                     "");
#endif /* ifdef WCOREDUMP */
                if (nf)
                        FREE_SPACE(sp, p, 0);
                return (1);
        }

        if (WIFEXITED(pstat) && WEXITSTATUS(pstat)) {
                /*
                 * Remain silent for "normal" errors when doing shell file
                 * name expansions, they almost certainly indicate nothing
                 * more than a failure to match.
                 *
                 * Remain silent for vi read filter errors.  It's historic
                 * practice.
                 */
                if (!silent) {
                        for (; isblank(*cmd); ++cmd);
                        p = msg_print(sp, cmd, &nf);
                        len = strlen(p);
                        msgq(sp, M_ERR, "%.*s%s: exited with status %d",
                            MINIMUM(len, 20), p, len > 20 ? " ..." : "",
                            WEXITSTATUS(pstat));
                        if (nf)
                                FREE_SPACE(sp, p, 0);
                }
                return (1);
        }
        return (0);
}
