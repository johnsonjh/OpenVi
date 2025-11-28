/*      $OpenBSD: ex_open.c,v 1.7 2016/01/06 22:28:52 millert Exp $     */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022-2024 Jeffrey H. Johnson
 *
 * See the LICENSE.md file for redistribution information.
 */

#include "../include/compat.h"

#include <sys/types.h>
#include <sys/queue.h>

#include <bitstring.h>
#include <limits.h>
#include <stdio.h>

#include "../common/common.h"

#undef open

/*
 * ex_open -- :[line] o[pen] [/pattern/] [flags]
 *
 *      Switch to single line "open" mode.
 *
 * PUBLIC: int ex_open(SCR *, EXCMD *);
 */
int
ex_open(SCR *sp, EXCMD *cmdp)
{
        /* If open option off, disallow open command. */
        if (!O_ISSET(sp, O_OPEN)) {
                msgq(sp, M_ERR,
            "The open command requires that the open option be set");
                return (1);
        }

        msgq(sp, M_ERR, "The open command is not yet implemented");
        return (1);
}
