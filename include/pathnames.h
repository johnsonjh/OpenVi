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

#ifndef _PATHNAMES_H
# define _PATHNAMES_H

# ifdef _PATH_EXRC
#  undef _PATH_EXRC
# endif /* ifdef _PATH_EXRC */

# define _PATH_EXRC ".exrc"

# ifdef _PATH_NEXRC
#  undef _PATH_NEXRC
# endif /* ifdef _PATH_NEXRC */

# define _PATH_NEXRC ".nexrc"

# ifdef _PATH_PRESERVE
#  undef _PATH_PRESERVE
# endif /* ifdef _PATH_PRESERVE */

# define _PATH_PRESERVE "/tmp/vi.recover"

# ifdef _PATH_SYSEXRC
#  undef _PATH_TAGS
# endif /* ifdef _PATH_SYSEXRC */

# define _PATH_SYSEXRC "/etc/vi.exrc"

# ifdef _PATH_TAGS
#  undef _PATH_TAGS
# endif /* ifdef _PATH_TAGS */

# define _PATH_TAGS "tags"

#endif /* ifndef _PATHNAMES_H */
