/*      $OpenBSD: unistd.h,v 1.103    2016/09/12 19:36:26 guenther Exp $ */
/*      $NetBSD:  unistd.h,v 1.26.4.1 1996/05/28 02:31:51 mrg Exp $      */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1991 The Regents of the University of California.
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
 *
 *      @(#)unistd.h    5.13 (Berkeley) 6/17/91
 */

#ifndef _COMPAT_UNISTD_H_
# define _COMPAT_UNISTD_H_

int      openbsd_pledge(const char *, const char *);
int      openbsd_getopt(int, char * const *, const char *);
int      openbsd_getopt(int, char * const *, const char *);
extern   char *openbsd_optarg;
extern   int openbsd_opterr, openbsd_optind, \
             openbsd_optopt, openbsd_optreset;

# ifdef __OpenBSD__
#  include <sys/types.h>
# else

#  include <sys/types.h>

#  define STDIN_FILENO  0       /* standard input file descriptor  */
#  define STDOUT_FILENO 1       /* standard output file descriptor */
#  define STDERR_FILENO 2       /* standard error file descriptor  */

# endif /* _COMPAT_UNISTD_H_ */

#endif /* ifdef __OpenBSD__ */

#define _COMPAT_GETOPT_H_       /* glibc includes getopt.h */
#include_next <unistd.h>
#undef  _COMPAT_GETOPT_H_
