/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 2022 Ørjan Malde <red@foxi.me>
 * Copyright (c) 2022-2024 Jeffrey H. Johnson <trnsz@pobox.com>
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

#include "../include/compat.h"

#include <errno.h>

#undef open

#if ( defined(__GLIBC__) && defined(__GLIBC_MINOR__) ) \
         || defined(__linux__) || defined(__midipix__) \
         || defined(__illumos__)
# include <sys/auxv.h>
#else
# include <unistd.h>
#endif /* if ( defined(__GLIBC__) && defined(__GLIBC_MINOR__) )
                  || defined(__linux__) || defined(__midipix__)
                  || defined(__illumos__) */

#if defined(__FreeBSD__) || defined(__OpenBSD__) \
       || ( defined(__APPLE__ ) && defined(__MACH__) ) \
       || defined(__CYGWIN__) || defined(__NetBSD__)
# include <unistd.h>
#else

# if defined(__linux__) || defined(__illumos__)
#  include <elf.h>
# endif /* if defined(__linux__) || defined(__illumos__) */

int
issetugid(void)
{
  int rv = 0;

  errno = 0;
# if !defined(_AIX) \
  && !defined(__illumos__) && !defined(__solaris__) \
  && !defined(__managarm__)
  rv = getauxval(AT_SECURE) != 0;
# endif /* if !defined(_AIX)
           && !defined(__illumos__) && !defined(__solaris__) */
  if (errno)
    {
      errno = 0;
      rv = 1;
    }

  return rv;
}
#endif /* if defined(__FreeBSD__) || defined(__OpenBSD__)
                || ( defined(__APPLE__) && defined(__MACH__) )
                || defined(__CYGWIN__) || defined(__NetBSD__) */
