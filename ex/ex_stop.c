/*      $OpenBSD: ex_stop.c,v 1.6 2014/11/12 04:28:41 bentley Exp $     */

/* SPDX-License-Identifier: BSD-3-Clause */

/*-
 * Copyright (c) 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1992, 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include <sys/types.h>
#include <sys/queue.h>

#include <bitstring.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <bsd_string.h>
#include <bsd_unistd.h>

#include "../common/common.h"

/*
 * ex_stop -- :stop[!]
 *            :suspend[!]
 *      Suspend execution.
 *
 * PUBLIC: int ex_stop(SCR *, EXCMD *);
 */
int
ex_stop(SCR *sp, EXCMD *cmdp)
{
        int allowed;

        /* For some strange reason, the force flag turns off autowrite. */
        if (!FL_ISSET(cmdp->iflags, E_C_FORCE) && file_aw(sp, FS_ALL))
                return (1);

        if (sp->gp->scr_suspend(sp, &allowed))
                return (1);
        if (!allowed)
                ex_emsg(sp, NULL, EXM_NOSUSPEND);
        return (0);
}
