/*      $OpenBSD: hash_func.c,v 1.11 2016/05/29 20:47:49 guenther Exp $ */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1990, 1993
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 2022-2024 Jeffrey H. Johnson
 *
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Margo Seltzer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "../../include/compat.h"

#include <sys/types.h>

#include <bsd_db.h>
#include <compat_bsd_db.h>

/* Chris Torek's hash function. */
u_int32_t
__default_hash(const void *key, size_t len)
{
        u_int32_t h, loop;
        u_int8_t *k;

#define HASH4a   h = (h << 5) - h + *k++;
#define HASH4b   h = (h << 5) + h + *k++;
#define HASH4 HASH4b

        h = 0;
        k = (u_int8_t *)key;
        if (len > 0) {
                loop = (len + 8 - 1) >> 3;

                switch (len & (8 - 1)) {
                case 0:
                        do {    /* All fall throughs */
                                HASH4;
                                /* FALLTHROUGH */
                case 7:
                                HASH4;
                                /* FALLTHROUGH */
                case 6:
                                HASH4;
                                /* FALLTHROUGH */
                case 5:
                                HASH4;
                                /* FALLTHROUGH */
                case 4:
                                HASH4;
                                /* FALLTHROUGH */
                case 3:
                                HASH4;
                                /* FALLTHROUGH */
                case 2:
                                HASH4;
                                /* FALLTHROUGH */
                case 1:
                                HASH4;
                                /* FALLTHROUGH */
                        } while (--loop);
                }

        }
        return (h);
}
