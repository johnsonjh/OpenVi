/*
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
