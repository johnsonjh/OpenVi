/*      $OpenBSD: line.c,v 1.17 2025/07/30 22:19:13 millert Exp $        */

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
#include <sys/time.h>

#include <bitstring.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <bsd_string.h>

#include "common.h"
#include "../vi/vi.h"

static int scr_update(SCR *, recno_t, lnop_t, int);

/*
 * db_eget --
 *      Front-end to db_get, special case handling for empty files.
 *
 * PUBLIC: int db_eget(SCR *, recno_t, char **, size_t *, int *);
 */

int
db_eget(SCR *sp, recno_t lno, char **pp, size_t *lenp, int *isemptyp)
{
        recno_t l1;

        if (isemptyp != NULL)
                *isemptyp = 0;

        /* If the line exists, simply return it. */
        if (!db_get(sp, lno, 0, pp, lenp))
                return (0);

        /*
         * If the user asked for line 0 or line 1, i.e. the only possible
         * line in an empty file, find the last line of the file; db_last
         * fails loudly.
         */

        if ((lno == OOBLNO || lno == 1) && db_last(sp, &l1))
                return (1);

        /* If the file isn't empty, fail loudly. */
        if ((lno != 0 && lno != 1) || l1 != 0) {
                db_err(sp, lno);
                return (1);
        }

        if (isemptyp != NULL)
                *isemptyp = 1;

        return (1);
}

/*
 * db_get --
 *      Look in the text buffers for a line, followed by the cache, followed
 *      by the database.
 *
 * PUBLIC: int db_get(SCR *, recno_t, u_int32_t, char **, size_t *);
 */

int
db_get(SCR *sp, recno_t lno, u_int32_t flags, char **pp, size_t *lenp)
{
        DBT data, key;
        EXF *ep;
        TEXT *tp;
        recno_t l1, l2;

        /*
         * The underlying recno stuff handles zero by returning NULL, but
         * have to have an OOB condition for the look-aside into the input
         * buffer anyway.
         */

        if (lno == OOBLNO)
                goto err1;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                goto err3;
        }

        if (LF_ISSET(DBG_NOCACHE))
                goto nocache;

        /*
         * Look-aside into the TEXT buffers and see if the line we want
         * is there.
         */

        if (F_ISSET(sp, SC_TINPUT)) {
                l1 = TAILQ_FIRST(&sp->tiq)->lno;
                l2 = TAILQ_LAST(&sp->tiq, _texth)->lno;
                if (l1 <= lno && l2 >= lno) {
                        TAILQ_FOREACH(tp, &sp->tiq, q) {
                                if (tp->lno == lno)
                                        break;
                        }
                        if (lenp != NULL)
                                *lenp = tp->len;
                        if (pp != NULL)
                                *pp = tp->lb;
                        return (0);
                }

                /*
                 * Adjust the line number for the number of lines used
                 * by the text input buffers.
                 */

                if (lno > l2)
                        lno -= l2 - l1;
        }

        /* Look-aside into the cache, and see if the line we want is there. */
        if (lno == ep->c_lno) {
                if (lenp != NULL)
                        *lenp = ep->c_len;
                if (pp != NULL)
                        *pp = ep->c_lp;
                return (0);
        }
        ep->c_lno = OOBLNO;

nocache:
        /* Get the line from the underlying database. */
        key.data = &lno;
        key.size = sizeof(lno);
        switch (ep->db->get(ep->db, &key, &data, 0)) {
        case -1:
                goto err2;
        case 1:
err1:           if (LF_ISSET(DBG_FATAL))
err2:                   db_err(sp, lno);
err3:           if (lenp != NULL)
                        *lenp = 0;
                if (pp != NULL)
                        *pp = NULL;
                return (1);
        }

        /* Reset the cache. */
        ep->c_lno = lno;
        ep->c_len = data.size;
        ep->c_lp = data.data;

        if (lenp != NULL)
                *lenp = data.size;
        if (pp != NULL)
                *pp = ep->c_lp;
        return (0);
}

/*
 * db_delete --
 *      Delete a line from the file.
 *
 * PUBLIC: int db_delete(SCR *, recno_t);
 */

int
db_delete(SCR *sp, recno_t lno)
{
        DBT key;
        EXF *ep;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                return (1);
        }

        /* Update marks, @ and global commands. */
        if (mark_insdel(sp, LINE_DELETE, lno))
                return (1);
        if (ex_g_insdel(sp, LINE_DELETE, lno))
                return (1);

        /* Log change. */
        log_line(sp, lno, LOG_LINE_DELETE);

        /* Update file. */
        key.data = &lno;
        key.size = sizeof(lno);
        if (ep->db->del(ep->db, &key, 0) == 1) {
                msgq(sp, M_SYSERR,
                    "unable to delete line %'lu", (unsigned long)lno);
                return (1);
        }

        /* Flush the cache, update line count, before screen update. */
        if (lno <= ep->c_lno)
                ep->c_lno = OOBLNO;
        if (ep->c_nlines != OOBLNO)
                --ep->c_nlines;

        /* File now modified. */
        if (F_ISSET(ep, F_FIRSTMODIFY))
                (void)rcv_init(sp);
        F_SET(ep, F_MODIFIED | F_RCV_SYNC);

        /* Update screen. */
        return (scr_update(sp, lno, LINE_DELETE, 1));
}

/*
 * db_append --
 *      Append a line into the file.
 *
 * PUBLIC: int db_append(SCR *, int, recno_t, char *, size_t);
 */

int
db_append(SCR *sp, int update, recno_t lno, char *p, size_t len)
{
        DBT data, key;
        EXF *ep;
        int rval;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                return (1);
        }

        /* Update file. */
        key.data = &lno;
        key.size = sizeof(lno);
        data.data = p;
        data.size = len;
        if (ep->db->put(ep->db, &key, &data, R_IAFTER) == -1) {
                msgq(sp, M_SYSERR,
                    "unable to append to line %'lu", (unsigned long)lno);
                return (1);
        }

        /* Flush the cache, update line count, before screen update. */
        if (lno < ep->c_lno)
                ep->c_lno = OOBLNO;
        if (ep->c_nlines != OOBLNO)
                ++ep->c_nlines;

        /* File now dirty. */
        if (F_ISSET(ep, F_FIRSTMODIFY))
                (void)rcv_init(sp);
        F_SET(ep, F_MODIFIED | F_RCV_SYNC);

        /* Log change. */
        log_line(sp, lno + 1, LOG_LINE_APPEND);

        /* Update marks, @ and global commands. */
        rval = 0;
        if (mark_insdel(sp, LINE_INSERT, lno + 1))
                rval = 1;
        if (ex_g_insdel(sp, LINE_INSERT, lno + 1))
                rval = 1;

        /*
         * Update screen.
         *
         * XXX
         * Nasty hack.  If multiple lines are input by the user, they aren't
         * committed until an <ESC> is entered.  The problem is the screen was
         * updated/scrolled as each line was entered.  So, when this routine
         * is called to copy the new lines from the cut buffer into the file,
         * it has to know not to update the screen again.
         */

        return (scr_update(sp, lno, LINE_APPEND, update) || rval);
}

/*
 * db_insert --
 *      Insert a line into the file.
 *
 * PUBLIC: int db_insert(SCR *, recno_t, char *, size_t);
 */

int
db_insert(SCR *sp, recno_t lno, char *p, size_t len)
{
        DBT data, key;
        EXF *ep;
        int rval;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                return (1);
        }

        /* Update file. */
        key.data = &lno;
        key.size = sizeof(lno);
        data.data = p;
        data.size = len;
        if (ep->db->put(ep->db, &key, &data, R_IBEFORE) == -1) {
                msgq(sp, M_SYSERR,
                    "unable to insert at line %'lu", (unsigned long)lno);
                return (1);
        }

        /* Flush the cache, update line count, before screen update. */
        if (lno >= ep->c_lno)
                ep->c_lno = OOBLNO;
        if (ep->c_nlines != OOBLNO)
                ++ep->c_nlines;

        /* File now dirty. */
        if (F_ISSET(ep, F_FIRSTMODIFY))
                (void)rcv_init(sp);
        F_SET(ep, F_MODIFIED | F_RCV_SYNC);

        /* Log change. */
        log_line(sp, lno, LOG_LINE_INSERT);

        /* Update marks, @ and global commands. */
        rval = 0;
        if (mark_insdel(sp, LINE_INSERT, lno))
                rval = 1;
        if (ex_g_insdel(sp, LINE_INSERT, lno))
                rval = 1;

        /* Update screen. */
        return (scr_update(sp, lno, LINE_INSERT, 1) || rval);
}

/*
 * db_set --
 *      Store a line in the file.
 *
 * PUBLIC: int db_set(SCR *, recno_t, char *, size_t);
 */

int
db_set(SCR *sp, recno_t lno, char *p, size_t len)
{
        DBT data, key;
        EXF *ep;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                return (1);
        }

        /* Log before change. */
        log_line(sp, lno, LOG_LINE_RESET_B);

        /* Update file. */
        key.data = &lno;
        key.size = sizeof(lno);
        data.data = p;
        data.size = len;
        if (ep->db->put(ep->db, &key, &data, 0) == -1) {
                msgq(sp, M_SYSERR,
                    "unable to store line %'lu", (unsigned long)lno);
                return (1);
        }

        /* Flush the cache, before logging or screen update. */
        if (lno == ep->c_lno)
                ep->c_lno = OOBLNO;

        /* File now dirty. */
        if (F_ISSET(ep, F_FIRSTMODIFY))
                (void)rcv_init(sp);
        F_SET(ep, F_MODIFIED | F_RCV_SYNC);

        /* Log after change. */
        log_line(sp, lno, LOG_LINE_RESET_F);

        /* Update screen. */
        return (scr_update(sp, lno, LINE_RESET, 1));
}

/*
 * db_exist --
 *      Return if a line exists.
 *
 * PUBLIC: int db_exist(SCR *, recno_t);
 */

int
db_exist(SCR *sp, recno_t lno)
{
        EXF *ep;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                return (1);
        }

        if (lno == OOBLNO)
                return (0);

        /*
         * Check the last-line number cache.  Adjust the cached line
         * number for the lines used by the text input buffers.
         */

        if (ep->c_nlines != OOBLNO)
                return (lno <= (F_ISSET(sp, SC_TINPUT) ?
                    ep->c_nlines + (TAILQ_LAST(&sp->tiq, _texth)->lno
                    - TAILQ_FIRST(&sp->tiq)->lno) : ep->c_nlines));

        /* Go get the line. */
        return (!db_get(sp, lno, 0, NULL, NULL));
}

/*
 * db_last --
 *      Return the number of lines in the file.
 *
 * PUBLIC: int db_last(SCR *, recno_t *);
 */

int
db_last(SCR *sp, recno_t *lnop)
{
        DBT data, key;
        EXF *ep;
        recno_t lno;

        /* Check for no underlying file. */
        if ((ep = sp->ep) == NULL) {
                ex_emsg(sp, NULL, EXM_NOFILEYET);
                return (1);
        }

        /*
         * Check the last-line number cache.  Adjust the cached line
         * number for the lines used by the text input buffers.
         */

        if (ep->c_nlines != OOBLNO) {
                *lnop = ep->c_nlines;
                if (F_ISSET(sp, SC_TINPUT))
                        *lnop += TAILQ_LAST(&sp->tiq, _texth)->lno -
                            TAILQ_FIRST(&sp->tiq)->lno;
                return (0);
        }

        key.data = &lno;
        key.size = sizeof(lno);

        switch (ep->db->seq(ep->db, &key, &data, R_LAST)) {
        case -1:
                msgq(sp, M_SYSERR, "unable to get last line");
                *lnop = 0;
                return (1);
        case 1:
                *lnop = 0;
                return (0);
        default:
                break;
        }

        /* Fill the cache. */
        memcpy(&lno, key.data, sizeof(lno));
        ep->c_nlines = ep->c_lno = lno;
        ep->c_len = data.size;
        ep->c_lp = data.data;

        /* Return the value. */
        *lnop = (F_ISSET(sp, SC_TINPUT) &&
            TAILQ_LAST(&sp->tiq, _texth)->lno > lno ?
            TAILQ_LAST(&sp->tiq, _texth)->lno : lno);
        return (0);
}

/*
 * db_err --
 *      Report a line error.
 *
 * PUBLIC: void db_err(SCR *, recno_t);
 */

void
db_err(SCR *sp, recno_t lno)
{
        if (lno)
            msgq(sp, M_ERR,
                "Error: unable to retrieve line %'lu", (unsigned long)lno);
}

/*
 * scr_update --
 *      Update all of the screens that are backed by the file that
 *      just changed.
 */

static int
scr_update(SCR *sp, recno_t lno, lnop_t op, int current)
{
        EXF *ep;
        SCR *tsp;

        if (F_ISSET(sp, SC_EX))
                return (0);

        ep = sp->ep;
        if (ep->refcnt != 1)
                TAILQ_FOREACH(tsp, &sp->gp->dq, q)
                        if (sp != tsp && tsp->ep == ep)
                                if (vs_change(tsp, lno, op))
                                        return (1);
        return (current ? vs_change(sp, lno, op) : 0);
}
