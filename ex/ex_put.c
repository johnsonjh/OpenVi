/*      $OpenBSD: ex_put.c,v 1.7 2025/08/23 21:02:10 millert Exp $      */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1992, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1992, 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022-2024 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include <sys/types.h>
#include <sys/queue.h>

#include <bitstring.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <bsd_string.h>

#include "../common/common.h"

/*
 * ex_put -- [line] pu[t] [buffer]
 *      Append a cut buffer into the file.
 *
 * PUBLIC: int ex_put(SCR *, EXCMD *);
 */
int
ex_put(SCR *sp, EXCMD *cmdp)
{
        MARK m;

        NEEDFILE(sp, cmdp);

        m.lno = sp->lno;
        m.cno = sp->cno;
        if (put(sp, NULL,
            FL_ISSET(cmdp->iflags, E_C_BUFFER) ? &cmdp->buffer : NULL,
            &cmdp->addr1, &m, 1, 1))
                return (1);
        sp->lno = m.lno;
        sp->cno = m.cno;
        return (0);
}
