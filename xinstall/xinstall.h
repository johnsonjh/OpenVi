/*
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
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
 * 3. Neither the names of the copyright holders nor the names of any
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _XINSTALL_H
# define _XINSTALL_H

# include "../include/compat.h"

# include <sys/stat.h>
# include <sys/types.h>

# include <grp.h>
# include <stdarg.h>
# include <stddef.h>

# undef open

# ifndef _PATH_STRIP
#  define _PATH_STRIP "/usr/bin/strip"
# endif /* ifndef _PATH_STRIP */

void   *openbsd_setmode(const char *p);
mode_t  openbsd_getmode(const void *bbox, mode_t omode);
int     gid_from_group(const char *name, gid_t *gid);
int     uid_from_user(const char *name, uid_t *uid);
void    openbsd_errc(int eval, int code, const char *fmt, ...);
void    openbsd_verrc(int eval, int code, const char *fmt, va_list ap);
void    openbsd_warnc(int code, const char *fmt, ...);
void    openbsd_vwarnc(int code, const char *fmt, va_list ap);
size_t  openbsd_strlcat(char *dst, const char *src, size_t dsize);

#endif /* ifndef _XINSTALL_H */
