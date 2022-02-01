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

#include_next <sys/time.h>

#ifndef _COMPAT_SYS_TIME_H_
# define _COMPAT_SYS_TIME_H_
# include <time.h>

# ifndef TIMEVAL_TO_TIMESPEC
#  define TIMEVAL_TO_TIMESPEC(tv, ts) {                                 \
        (ts)->tv_sec = (tv)->tv_sec;                                    \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;                           \
}
# endif /* ifndef TIMEVAL_TO_TIMESPEC */

# ifndef TIMESPEC_TO_TIMEVAL
#  define TIMESPEC_TO_TIMEVAL(tv, ts) {                                 \
        (tv)->tv_sec = (ts)->tv_sec;                                    \
        (tv)->tv_usec = (ts)->tv_nsec / 1000;                           \
}
# endif /* ifndef TIMESPEC_TO_TIMEVAL */

# ifdef timespecclear
#  undef timespecclear
# endif /* ifdef timespecclear */
# define timespecclear(tsp)             (tsp)->tv_sec = (tsp)->tv_nsec = 0

# ifdef timespecisset
#  undef timespecisset
# endif /* ifdef timespecisset */
# define timespecisset(tsp)             ((tsp)->tv_sec || (tsp)->tv_nsec)

# ifdef timespeccmp
#  undef timespeccmp
# endif /* ifdef timespeccmp */
# define timespeccmp(tsp, usp, cmp)                                     \
        (((tsp)->tv_sec == (usp)->tv_sec) ?                             \
            ((tsp)->tv_nsec cmp (usp)->tv_nsec) :                       \
            ((tsp)->tv_sec cmp (usp)->tv_sec))

# ifdef timespecadd
#  undef timespecadd
# endif /* ifdef timespecadd */
# define timespecadd(tsp, usp, vsp)                                     \
        do {                                                            \
                (vsp)->tv_sec = (tsp)->tv_sec + (usp)->tv_sec;          \
                (vsp)->tv_nsec = (tsp)->tv_nsec + (usp)->tv_nsec;       \
                if ((vsp)->tv_nsec >= 1000000000L) {                    \
                        (vsp)->tv_sec++;                                \
                        (vsp)->tv_nsec -= 1000000000L;                  \
                }                                                       \
        } while (0)

# ifdef timespecsub
#  undef timespecsub
# endif /* ifdef timespecsub */
# define timespecsub(tsp, usp, vsp)                                     \
        do {                                                            \
                (vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;          \
                (vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec;       \
                if ((vsp)->tv_nsec < 0) {                               \
                        (vsp)->tv_sec--;                                \
                        (vsp)->tv_nsec += 1000000000L;                  \
                }                                                       \
        } while (0)

#endif /* _COMPAT_SYS_TIME_H_ */
