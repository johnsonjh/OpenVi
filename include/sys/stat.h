/*
 * Copyright (c) 2022 Jeffrey. H. Johnson <trnsz@pobox.com>
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

#ifndef _COMPAT_SYS_STAT_H_
# define _COMPAT_SYS_STAT_H_

# ifndef S_ISTXT
#  define S_ISTXT  S_ISVTX	/* sticky bit */
# endif /* ifndef S_ISTXT */

# ifndef st_atimespec
#  define st_atimespec		st_atim
# endif /* ifndef st_atimespec */

# ifndef st_atimensec
#  define st_atimensec		st_atim.tv_nsec
# endif /* ifndef st_atimensec */

# ifndef st_mtimespec
#  define st_mtimespec		st_mtim
# endif /* ifndef st_mtimespec */

# ifndef st_mtimensec
#  define st_mtimensec		st_mtim.tv_nsec
# endif /* ifndef st_mtimensec */

# ifndef st_ctimespec
#  define st_ctimespec		st_ctim
# endif /* ifndef st_ctimespec */

# ifndef st_ctimensec
#  define st_ctimensec		st_ctim.tv_nsec
# endif /* ifndef st_ctimensec */

#endif /* !_COMPAT_SYS_STAT_H_ */

#include_next <sys/stat.h>
