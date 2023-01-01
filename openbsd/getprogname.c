/* $OpenBSD: getprogname.c,v 1.4 2016/03/13 18:34:20 guenther Exp $ */

/* SPDX-License-Identifier: ISC */

/*
 * Copyright (c) 2013 Antoine Jacoutot <ajacoutot@openbsd.org>
 * Copyright (c) 2022-2023 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <bsd_stdlib.h>

#ifdef _AIX
# include <bsd_unistd.h>
# include <bsd_string.h>
# include <procinfo.h>
#else
extern char *__progname;
#endif /* ifdef _AIX */

const char *
bsd_getprogname(void)
{
#ifdef _AIX
  static char *p;
  static int first = 1;
  if (first)
    {
      first = 0;
      pid_t pid = getpid();
      struct procentry64 procs;
      p = (0 < getprocs64 (&procs, sizeof procs, NULL, 0, &pid, 1)
           ? strdup (procs.pi_comm) : NULL);
      if (!p)
        p = "?";
    }
  return p;
#else
  return (__progname);
#endif /* ifdef _AIX */
}
