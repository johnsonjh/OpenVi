/*         $OpenBSD: verr.c,v 1.11 2016/03/13 18:34:20 guenther Exp $        */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1993 The Regents of the University of California.
 * Copyright (c) 2022-2023 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * All rights reserved.
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

#include "../include/compat.h"

#include "errc.h"

#include <bsd_err.h>
#include <errno.h>
#include <stdio.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <stdarg.h>

#undef open

void
openbsd_verr(int eval, const char *fmt, va_list ap)
{
  int sverrno;

  sverrno = errno;
  (void)fprintf(stderr, "%s: ", bsd_getprogname());
  if (fmt != NULL) {
    (void)vfprintf(stderr, fmt, ap);
    (void)fprintf(stderr, ": ");
  }
  (void)fprintf(stderr, "%s\n", strerror(sverrno));
  exit(eval);
}
