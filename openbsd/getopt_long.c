/*      $OpenBSD: getopt_long.c,v 1.32 2020/05/27 22:25:09 schwarze Exp $       */
/*      $NetBSD: getopt_long.c,v 1.15 2002/01/31 22:43:40 tv Exp $      */

/*
 * Copyright (c) 2002 Todd C. Miller <millert@openbsd.org>
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
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
 *
 * Sponsored in part by the Defense Advanced Research Projects
 * Agency (DARPA) and Air Force Research Laboratory, Air Force
 * Materiel Command, USAF, under agreement number F39502-99-1-0512.
 */

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Dieter Baron and Thomas Klausner.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "../include/compat.h"

#include "libgen.h"
#include "getopt_long.h"

#include <bsd_err.h>
#include <errno.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <bsd_unistd.h>

#include "errc.h"

#undef open

int   openbsd_opterr = 1;    /* if error message should be printed */
int   openbsd_optind = 1;    /* index into parent argv vector */
int   openbsd_optopt = '?';  /* character checked for validity */
int   openbsd_optreset;      /* reset getopt */
char *openbsd_optarg;        /* argument associated with option */

#define PRINT_ERROR (( openbsd_opterr ) && ( *options != ':' ))

#define FLAG_PERMUTE  0x01  /* permute non-options to the end of argv */
#define FLAG_ALLARGS  0x02  /* treat non-options as args to option "-1" */
#define FLAG_LONGONLY 0x04  /* operate as getopt_long_only */

/* return values */
#define BADCH (int)'?'
#define BADARG (( *options == ':' ) ? (int)':' : (int)'?' )
#define INORDER (int)1

#define EMSG ""

static int openbsd_getopt_internal(int, char *const *, const char *,
                                   const struct option *, int *, int);

static int openbsd_parse_long_options(char *const *nargv, const char *options,
                                      const struct option *long_options,
                                      int *idx, int short_too, int flags);
static int openbsd_gcd(int, int);
static void openbsd_permute_args(int, int, int, char *const *);

static char *place = EMSG; /* option letter processing */

static int openbsd_nonopt_start = -1; /* first non option argument (for permute) */
static int openbsd_nonopt_end   = -1; /* first option after non options (for permute) */

/* Error messages */
static const char openbsd_recargchar[]   = "option requires an argument -- %c";
static const char openbsd_recargstring[] = "option requires an argument -- %s";
static const char openbsd_ambig[]        = "ambiguous option -- %.*s";
static const char openbsd_noarg[]        = "option doesn't take an argument -- %.*s";
static const char openbsd_illoptchar[]   = "unknown option -- %c";
static const char openbsd_illoptstring[] = "unknown option -- %s";

/*
 * Compute the greatest common divisor of a and b.
 */
static int
openbsd_gcd(int a, int b)
{
  int c;

  c = a % b;
  while (c != 0)
    {
      a = b;
      b = c;
      c = a % b;
    }

  return b;
}

/*
 * Exchange the block from nonopt_start to nonopt_end with the block
 * from nonopt_end to opt_end (keeping the same order of arguments
 * in each block).
 */
static void
openbsd_permute_args(int panonopt_start, int panonopt_end, int opt_end,
                     char *const *nargv)
{
  int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
  char *swap;

  /*
   * compute lengths of blocks and number and size of cycles
   */
  nnonopts = panonopt_end - panonopt_start;
  nopts = opt_end - panonopt_end;
  ncycle = openbsd_gcd(nnonopts, nopts);
  cyclelen = ( opt_end - panonopt_start ) / ncycle;

  for (i = 0; i < ncycle; i++)
    {
      cstart = panonopt_end + i;
      pos = cstart;
      for (j = 0; j < cyclelen; j++)
        {
          if (pos >= panonopt_end)
            {
              pos -= nnonopts;
            }
          else
            {
              pos += nopts;
            }

          swap = nargv[pos];
          ((char **)nargv )[pos] = nargv[cstart];
          ((char **)nargv )[cstart] = swap;
        }
    }
}

/*
 * openbsd_parse_long_options --
 *      Parse long options in argc/argv argument vector.
 * Returns -1 if short_too is set and the option does not match long_options.
 */
static int
openbsd_parse_long_options(char *const *nargv, const char *options,
                           const struct option *long_options, int *idx,
                           int short_too, int flags)
{
  char *current_argv, *has_equal;
  size_t current_argv_len;
  int i, match, exact_match, second_partial_match;

  current_argv = place;
  match = -1;
  exact_match = 0;
  second_partial_match = 0;

  openbsd_optind++;

  if (( has_equal = strchr(current_argv, '=')) != NULL)
    {
      /* argument found (--option=arg) */
      current_argv_len = has_equal - current_argv;
      has_equal++;
    }
  else
    {
      current_argv_len = strlen(current_argv);
    }

  for (i = 0; long_options[i].name; i++)
    {
      /* find matching long option */
      if (strncmp(current_argv, long_options[i].name, current_argv_len))
        {
          continue;
        }

      if (strlen(long_options[i].name) == current_argv_len)
        {
          /* exact match */
          match = i;
          exact_match = 1;
          break;
        }

      /*
       * If this is a known short option, don't allow
       * a partial match of a single character.
       */
      if (short_too && current_argv_len == 1)
        {
          continue;
        }

      if (match == -1) /* first partial match */
        {
          match = i;
        }
      else if (( flags & FLAG_LONGONLY )
               || long_options[i].has_arg != long_options[match].has_arg
               || long_options[i].flag != long_options[match].flag
               || long_options[i].val != long_options[match].val)
        {
          second_partial_match = 1;
        }
    }

  if (!exact_match && second_partial_match)
    {
      /* ambiguous abbreviation */
      if (PRINT_ERROR)
        {
          openbsd_warnx(openbsd_ambig, (int)current_argv_len, current_argv);
        }

      openbsd_optopt = 0;
      return BADCH;
    }

  if (match != -1)
    { /* option found */
      if (long_options[match].has_arg == no_argument && has_equal)
        {
          if (PRINT_ERROR)
            {
              openbsd_warnx(openbsd_noarg, (int)current_argv_len, current_argv);
            }

          /*
           * XXX: GNU sets optopt to val regardless of flag
           */
          if (long_options[match].flag == NULL)
            {
              openbsd_optopt = long_options[match].val;
            }
          else
            {
              openbsd_optopt = 0;
            }

          return BADARG;
        }

      if (long_options[match].has_arg == required_argument
          || long_options[match].has_arg == optional_argument)
        {
          if (has_equal)
            {
              openbsd_optarg = has_equal;
            }
          else if (long_options[match].has_arg == required_argument)
            {
              /*
               * optional argument doesn't use next nargv
               */
              openbsd_optarg = nargv[openbsd_optind++];
            }
        }

      if (( long_options[match].has_arg == required_argument ) && ( openbsd_optarg == NULL ))
        {
          /*
           * Missing argument; leading ':' indicates no error
           * should be generated.
           */
          if (PRINT_ERROR)
            {
              openbsd_warnx(openbsd_recargstring, current_argv);
            }

          /*
           * XXX: GNU sets optopt to val regardless of flag
           */
          if (long_options[match].flag == NULL)
            {
              openbsd_optopt = long_options[match].val;
            }
          else
            {
              openbsd_optopt = 0;
            }

          --openbsd_optind;
          return BADARG;
        }
    }
  else
    { /* unknown option */
      if (short_too)
        {
          --openbsd_optind;
          return -1;
        }

      if (PRINT_ERROR)
        {
          openbsd_warnx(openbsd_illoptstring, current_argv);
        }

      openbsd_optopt = 0;
      return BADCH;
    }

  if (idx)
    {
      *idx = match;
    }

  if (long_options[match].flag)
    {
      *long_options[match].flag = long_options[match].val;
      return 0;
    }
  else
    {
      return long_options[match].val;
    }
}

/*
 * openbsd_getopt_internal --
 *      Parse argc/argv argument vector.  Called by user level routines.
 */
static int
openbsd_getopt_internal(int nargc, char *const *nargv, const char *options,
                        const struct option *long_options, int *idx, int flags)
{
  char *oli; /* option letter list index */
  int openbsd_optchar, short_too;
  static int posixly_correct = -1;

  if (options == NULL)
    {
      return -1;
    }

  /*
   * Some GNU programs (like cvs) set optind to 0 instead of
   * using optreset.  Work around this braindamage.
   */
  if (openbsd_optind == 0)
    {
      openbsd_optind = openbsd_optreset = 1;
    }

  /*
   * Disable GNU extensions if POSIXLY_CORRECT or POSIX_ME_HARDER
   * is set, or if the options string begins with a '+'.
   */
  if (posixly_correct == -1 || openbsd_optreset)
    {
      posixly_correct = ( getenv("POSIXLY_CORRECT") != NULL );
    }

  if (posixly_correct == -1 || openbsd_optreset)
    {
      posixly_correct = ( getenv("POSIX_ME_HARDER") != NULL );
    }

  if (*options == '-')
    {
      flags |= FLAG_ALLARGS;
    }
  else if (posixly_correct || *options == '+')
    {
      flags &= ~FLAG_PERMUTE;
    }

  if (*options == '+' || *options == '-')
    {
      options++;
    }

  openbsd_optarg = NULL;
  if (openbsd_optreset)
    {
      openbsd_nonopt_start = openbsd_nonopt_end = -1;
    }

start:
  if (openbsd_optreset || !*place)
    { /* update scanning pointer */
      openbsd_optreset = 0;
      if (openbsd_optind >= nargc)
        { /* end of argument vector */
          place = EMSG;
          if (openbsd_nonopt_end != -1)
            {
              /* do permutation, if we have to */
              openbsd_permute_args(openbsd_nonopt_start, openbsd_nonopt_end, openbsd_optind, nargv);
              openbsd_optind -= openbsd_nonopt_end - openbsd_nonopt_start;
            }
          else if (openbsd_nonopt_start != -1)
            {
              /*
               * If we skipped non-options, set optind
               * to the first of them.
               */
              openbsd_optind = openbsd_nonopt_start;
            }

          openbsd_nonopt_start = openbsd_nonopt_end = -1;
          return -1;
        }

      if (*( place = nargv[openbsd_optind] ) != '-'
          || ( place[1] == '\0' && strchr(options, '-') == NULL ))
        {
          place = EMSG; /* found non-option */
          if (flags & FLAG_ALLARGS)
            {
              /*
               * GNU extension:
               * return non-option as argument to option 1
               */
              openbsd_optarg = nargv[openbsd_optind++];
              return INORDER;
            }

          if (!( flags & FLAG_PERMUTE ))
            {
              /*
               * If no permutation wanted, stop parsing
               * at first non-option.
               */
              return -1;
            }

          /* do permutation */
          if (openbsd_nonopt_start == -1)
            {
              openbsd_nonopt_start = openbsd_optind;
            }
          else if (openbsd_nonopt_end != -1)
            {
              openbsd_permute_args(openbsd_nonopt_start, openbsd_nonopt_end, openbsd_optind, nargv);
              openbsd_nonopt_start = openbsd_optind - ( openbsd_nonopt_end - openbsd_nonopt_start );
              openbsd_nonopt_end = -1;
            }

          openbsd_optind++;
          /* process next argument */
          goto start;
        }

      if (openbsd_nonopt_start != -1 && openbsd_nonopt_end == -1)
        {
          openbsd_nonopt_end = openbsd_optind;
        }

      /*
       * If we have "-" do nothing, if "--" we are done.
       */
      if (place[1] != '\0' && *++place == '-' && place[1] == '\0')
        {
          openbsd_optind++;
          place = EMSG;
          /*
           * We found an option (--), so if we skipped
           * non-options, we have to permute.
           */
          if (openbsd_nonopt_end != -1)
            {
              openbsd_permute_args(openbsd_nonopt_start, openbsd_nonopt_end, openbsd_optind, nargv);
              openbsd_optind -= openbsd_nonopt_end - openbsd_nonopt_start;
            }

          openbsd_nonopt_start = openbsd_nonopt_end = -1;
          return -1;
        }
    }

  /*
   * Check long options if:
   *  1) we were passed some
   *  2) the arg is not just "-"
   *  3) either the arg starts with -- we are openbsd_getopt_long_only()
   */
  if (long_options != NULL && place != nargv[openbsd_optind]
      && ( *place == '-' || ( flags & FLAG_LONGONLY )))
    {
      short_too = 0;
      if (*place == '-')
        {
          place++; /* --foo long option */
        }
      else if (*place != ':' && strchr(options, *place) != NULL)
        {
          short_too = 1; /* could be short option too */
        }

      openbsd_optchar = openbsd_parse_long_options(
        nargv,
        options,
        long_options,
        idx,
        short_too,
        flags);
      if (openbsd_optchar != -1)
        {
          place = EMSG;
          return openbsd_optchar;
        }
    }

  if (( openbsd_optchar = (int)*place++ ) == (int)':'
      || ( oli = strchr(options, openbsd_optchar)) == NULL)
    {
      if (!*place)
        {
          ++openbsd_optind;
        }

      if (PRINT_ERROR)
        {
          openbsd_warnx(openbsd_illoptchar, openbsd_optchar);
        }

      openbsd_optopt = openbsd_optchar;
      return BADCH;
    }

  if (long_options != NULL && openbsd_optchar == 'W' && oli[1] == ';')
    {
      /* -W long-option */
      if (*place) /* no space */
      /* NOTHING */ {
          ;
        }
      else if (++openbsd_optind >= nargc)
        { /* no arg */
          place = EMSG;
          if (PRINT_ERROR)
            {
              openbsd_warnx(openbsd_recargchar, openbsd_optchar);
            }

          openbsd_optopt = openbsd_optchar;
          return BADARG;
        }
      else /* white space */
        {
          place = nargv[openbsd_optind];
        }

      openbsd_optchar = openbsd_parse_long_options(nargv, options, long_options,
                                           idx, 0, flags);
      place = EMSG;
      return openbsd_optchar;
    }

  if (*++oli != ':')
    { /* doesn't take argument */
      if (!*place)
        {
          ++openbsd_optind;
        }
    }
  else
    { /* takes (optional) argument */
      openbsd_optarg = NULL;
      if (*place) /* no white space */
        {
          openbsd_optarg = place;
        }
      else if (oli[1] != ':')
        { /* arg not optional */
          if (++openbsd_optind >= nargc)
            { /* no arg */
              place = EMSG;
              if (PRINT_ERROR)
                {
                  openbsd_warnx(openbsd_recargchar, openbsd_optchar);
                }

              openbsd_optopt = openbsd_optchar;
              return BADARG;
            }
          else
            {
              openbsd_optarg = nargv[openbsd_optind];
            }
        }

      place = EMSG;
      ++openbsd_optind;
    }

  /* dump back option letter */
  return openbsd_optchar;
}

/*
 * opembsd_getopt --
 *      Parse argc/argv argument vector.
 */
int
openbsd_getopt(int nargc, char *const *nargv, const char *options)
{
  /*
   * We don't pass FLAG_PERMUTE to openbsd_getopt_internal() since
   * the BSD getopt(3) (unlike GNU) has never done this.
   *
   * Furthermore, since many privileged programs call openbsd_getopt()
   * before dropping privileges it makes sense to keep things
   * as simple (and bug-free) as possible.
   */
  return openbsd_getopt_internal(nargc, nargv, options, NULL, NULL, 0);
}

/*
 * openbsd_getopt_long --
 *      Parse argc/argv argument vector.
 */
int
openbsd_getopt_long(int nargc, char *const *nargv, const char *options,
                    const struct option *long_options, int *idx)
{
  return openbsd_getopt_internal(
    nargc,
    nargv,
    options,
    long_options,
    idx,
    FLAG_PERMUTE);
}

/*
 * getopt_long_only --
 *      Parse argc/argv argument vector.
 */
int
openbsd_getopt_long_only(int nargc, char *const *nargv, const char *options,
                         const struct option *long_options, int *idx)
{
  return openbsd_getopt_internal(
    nargc,
    nargv,
    options,
    long_options,
    idx,
    FLAG_PERMUTE | FLAG_LONGONLY);
}
