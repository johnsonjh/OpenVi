/*      $OpenBSD: proc.h,v 1.235 2017/02/14 10:31:15 mpi Exp $  */
/*      $NetBSD: proc.h,v 1.44 1996/04/22 01:23:21 christos Exp $       */

/*-
 * Copyright (c) 1986, 1989, 1991, 1993
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 * Copyright (c) 1989, 1990, 1991, 1992, 1993 UNIX System Laboratories, Inc.
 *
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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
 *      @(#)proc.h      8.8 (Berkeley) 1/21/94
 */

#ifndef _SYS_PROC_H_
# define _SYS_PROC_H_

# include <sys/queue.h>

/*
 * These flags are kept in ps_flags.
 */
# define PS_CONTROLT    0x00000001      /* Has a controlling terminal. */
# define PS_EXEC                0x00000002      /* Process called exec. */
# define PS_INEXEC      0x00000004      /* Process is doing an exec right now */
# define PS_EXITING     0x00000008      /* Process is exiting. */
# define PS_SUGID       0x00000010      /* Had set id privs since last exec. */
# define PS_SUGIDEXEC   0x00000020      /* last execve() was set[ug]id */
# define PS_PPWAIT      0x00000040      /* Parent waits for exec/exit. */
# define PS_ISPWAIT     0x00000080      /* Is parent of PPWAIT child. */
# define PS_PROFIL      0x00000100      /* Has started profiling. */
# define PS_TRACED      0x00000200      /* Being ptraced. */
# define PS_WAITED      0x00000400      /* Stopped proc was waited for. */
# define PS_COREDUMP    0x00000800      /* Busy coredumping */
# define PS_SINGLEEXIT  0x00001000      /* Other threads must die. */
# define PS_SINGLEUNWIND        0x00002000      /* Other threads must unwind. */
# define PS_NOZOMBIE    0x00004000      /* No signal or zombie at exit. */
# define PS_STOPPED     0x00008000      /* Just stopped, need sig to parent. */
# define PS_SYSTEM      0x00010000      /* No sigs, stats or swapping. */
# define PS_EMBRYO      0x00020000      /* New process, not yet fledged */
# define PS_ZOMBIE      0x00040000      /* Dead and ready to be waited for */
# define PS_NOBROADCASTKILL 0x00080000  /* Process excluded from kill -1. */
# define PS_PLEDGE      0x00100000      /* Has called pledge(2) */
# define PS_WXNEEDED    0x00200000      /* Process may violate W^X */

# define PS_BITS \
    ("\20" "\01CONTROLT" "\02EXEC" "\03INEXEC" "\04EXITING" "\05SUGID" \
     "\06SUGIDEXEC" "\07PPWAIT" "\010ISPWAIT" "\011PROFIL" "\012TRACED" \
     "\013WAITED" "\014COREDUMP" "\015SINGLEEXIT" "\016SINGLEUNWIND" \
     "\017NOZOMBIE" "\020STOPPED" "\021SYSTEM" "\022EMBRYO" "\023ZOMBIE" \
     "\024NOBROADCASTKILL" "\025PLEDGE" "\026WXNEEDED")

/* Status values. */
# define SIDL   1               /* Thread being created by fork. */
# define SRUN   2               /* Currently runnable. */
# define SSLEEP 3               /* Sleeping on an address. */
# define SSTOP  4               /* Debugging or suspension. */
# define SZOMB  5               /* unused */
# define SDEAD  6               /* Thread is almost gone */
# define SONPROC        7               /* Thread is currently on a CPU. */

# define P_ZOMBIE(p)    ((p)->p_stat == SDEAD)
# define P_HASSIBLING(p)        (TAILQ_FIRST(&(p)->p_p->ps_threads) != (p) || \
                         TAILQ_NEXT((p), p_thr_link) != NULL)

/*
 * These flags are per-thread and kept in p_flag
 */
# define P_INKTR                0x00000001      /* In a ktrace op, don't recurse */
# define P_PROFPEND     0x00000002      /* SIGPROF needs to be posted */
# define P_ALRMPEND     0x00000004      /* SIGVTALRM needs to be posted */
# define P_SIGSUSPEND   0x00000008      /* Need to restore before-suspend mask*/
# define P_CANTSLEEP    0x00000010      /* insomniac thread */
# define P_SELECT       0x00000040      /* Selecting; wakeup/waiting danger. */
# define P_SINTR                0x00000080      /* Sleep is interruptible. */
# define P_SYSTEM       0x00000200      /* No sigs, stats or swapping. */
# define P_TIMEOUT      0x00000400      /* Timing out during sleep. */
# define P_WEXIT                0x00002000      /* Working on exiting. */
# define P_OWEUPC       0x00008000      /* Owe proc an addupc() at next ast. */
# define P_SUSPSINGLE   0x00080000      /* Need to stop for single threading. */
# define P_CONTINUED    0x00800000      /* Proc has continued from a stopped state. */
# define P_THREAD       0x04000000      /* Only a thread, not a real process */
# define P_SUSPSIG      0x08000000      /* Stopped from signal. */
# define P_SOFTDEP      0x10000000      /* Stuck processing softdep worklist */
# define P_CPUPEG       0x40000000      /* Do not move to another cpu. */

# define P_BITS \
    ("\20" "\01INKTR" "\02PROFPEND" "\03ALRMPEND" "\04SIGSUSPEND" \
     "\05CANTSLEEP" "\07SELECT" "\010SINTR" "\012SYSTEM" "\013TIMEOUT" \
     "\016WEXIT" "\020OWEUPC" "\024SUSPSINGLE" "\027XX" \
     "\030CONTINUED" "\033THREAD" "\034SUSPSIG" "\035SOFTDEP" "\037CPUPEG")

# define THREAD_PID_OFFSET      100000

#endif  /* !_SYS_PROC_H_ */

