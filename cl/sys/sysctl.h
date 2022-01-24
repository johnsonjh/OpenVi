/*	$OpenBSD: sysctl.h,v 1.171 2017/01/21 05:42:03 guenther Exp $	*/
/*	$NetBSD: sysctl.h,v 1.16 1996/04/09 20:55:36 cgd Exp $	*/

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Karels at Berkeley Software Design, Inc.
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
 *	@(#)sysctl.h	8.2 (Berkeley) 3/30/95
 */

#ifndef _COMPAT_SYS_SYSCTL_H_
#define	_COMPAT_SYS_SYSCTL_H_

#include <stdint.h>

/*
 * KERN_PROC subtypes
 */
#define	KERN_PROC_ALL		0	/* everything but kernel threads */
#define	KERN_PROC_PID		1	/* by process id */
#define	KERN_PROC_PGRP		2	/* by process group id */
#define	KERN_PROC_SESSION	3	/* by session of pid */
#define	KERN_PROC_TTY		4	/* by controlling tty */
#define	KERN_PROC_UID		5	/* by effective uid */
#define	KERN_PROC_RUID		6	/* by real uid */
#define	KERN_PROC_KTHREAD	7	/* also return kernel threads */
#define	KERN_PROC_SHOW_THREADS	0x40000000/* also return normal threads */

/*
 * KERN_PROC_ARGS subtypes
 */
#define KERN_PROC_ARGV		1
#define KERN_PROC_NARGV		2
#define KERN_PROC_ENV		3
#define KERN_PROC_NENV		4

/*
 * KERN_PROC subtype ops return arrays of relatively fixed size
 * structures of process info.   Use 8 byte alignment, and new
 * elements should only be added to the end of this structure so
 * binary compatibility can be preserved.
 */
#define	KI_NGROUPS	16
#define	KI_MAXCOMLEN	24	/* extra for 8 byte alignment */
#define	KI_WMESGLEN	8
#define	KI_MAXLOGNAME	32
#define	KI_EMULNAMELEN	8

#define KI_NOCPU	(~(u_int64_t)0)

struct kinfo_proc {
	u_int64_t p_forw;		/* PTR: linked run/sleep queue. */
	u_int64_t p_back;
	u_int64_t p_paddr;		/* PTR: address of proc */

	u_int64_t p_addr;		/* PTR: Kernel virtual addr of u-area */
	u_int64_t p_fd;			/* PTR: Ptr to open files structure. */
	u_int64_t p_stats;		/* unused, always zero. */
	u_int64_t p_limit;		/* PTR: Process limits. */
	u_int64_t p_vmspace;		/* PTR: Address space. */
	u_int64_t p_sigacts;		/* PTR: Signal actions, state */
	u_int64_t p_sess;		/* PTR: session pointer */
	u_int64_t p_tsess;		/* PTR: tty session pointer */
	u_int64_t p_ru;			/* PTR: Exit information. XXX */

	int32_t	p_eflag;		/* LONG: extra kinfo_proc flags */
#define	EPROC_CTTY	0x01	/* controlling tty vnode active */
#define	EPROC_SLEADER	0x02	/* session leader */
	int32_t	p_exitsig;		/* unused, always zero. */
	int32_t	p_flag;			/* INT: P_* flags. */

	int32_t	p_pid;			/* PID_T: Process identifier. */
	int32_t	p_ppid;			/* PID_T: Parent process id */
	int32_t	p_sid;			/* PID_T: session id */
	int32_t	p__pgid;		/* PID_T: process group id */
					/* XXX: <sys/proc.h> hijacks p_pgid */
	int32_t	p_tpgid;		/* PID_T: tty process group id */

	u_int32_t p_uid;		/* UID_T: effective user id */
	u_int32_t p_ruid;		/* UID_T: real user id */
	u_int32_t p_gid;		/* GID_T: effective group id */
	u_int32_t p_rgid;		/* GID_T: real group id */

	u_int32_t p_groups[KI_NGROUPS];	/* GID_T: groups */
	int16_t	p_ngroups;		/* SHORT: number of groups */

	int16_t	p_jobc;			/* SHORT: job control counter */
	u_int32_t p_tdev;		/* DEV_T: controlling tty dev */

	u_int32_t p_estcpu;		/* U_INT: Time averaged value of p_cpticks. */
	u_int32_t p_rtime_sec;		/* STRUCT TIMEVAL: Real time. */
	u_int32_t p_rtime_usec;		/* STRUCT TIMEVAL: Real time. */
	int32_t	p_cpticks;		/* INT: Ticks of cpu time. */
	u_int32_t p_pctcpu;		/* FIXPT_T: %cpu for this process */
	u_int32_t p_swtime;		/* unused, always zero */
	u_int32_t p_slptime;		/* U_INT: Time since last blocked. */
	int32_t	p_schedflags;		/* INT: PSCHED_* flags */

	u_int64_t p_uticks;		/* U_QUAD_T: Statclock hits in user mode. */
	u_int64_t p_sticks;		/* U_QUAD_T: Statclock hits in system mode. */
	u_int64_t p_iticks;		/* U_QUAD_T: Statclock hits processing intr. */

	u_int64_t p_tracep;		/* PTR: Trace to vnode or file */
	int32_t	p_traceflag;		/* INT: Kernel trace points. */

	int32_t p_holdcnt;		/* INT: If non-zero, don't swap. */

	int32_t p_siglist;		/* INT: Signals arrived but not delivered. */
	u_int32_t p_sigmask;		/* SIGSET_T: Current signal mask. */
	u_int32_t p_sigignore;		/* SIGSET_T: Signals being ignored. */
	u_int32_t p_sigcatch;		/* SIGSET_T: Signals being caught by user. */

	int8_t	p_stat;			/* CHAR: S* process status (from LWP). */
	u_int8_t p_priority;		/* U_CHAR: Process priority. */
	u_int8_t p_usrpri;		/* U_CHAR: User-priority based on p_estcpu and ps_nice. */
	u_int8_t p_nice;		/* U_CHAR: Process "nice" value. */

	u_int16_t p_xstat;		/* U_SHORT: Exit status for wait; also stop signal. */
	u_int16_t p_acflag;		/* U_SHORT: Accounting flags. */

	char	p_comm[KI_MAXCOMLEN];

	char	p_wmesg[KI_WMESGLEN];	/* wchan message */
	u_int64_t p_wchan;		/* PTR: sleep address. */

	char	p_login[KI_MAXLOGNAME];	/* setlogin() name */

	int32_t	p_vm_rssize;		/* SEGSZ_T: current resident set size in pages */
	int32_t	p_vm_tsize;		/* SEGSZ_T: text size (pages) */
	int32_t	p_vm_dsize;		/* SEGSZ_T: data size (pages) */
	int32_t	p_vm_ssize;		/* SEGSZ_T: stack size (pages) */

	int64_t	p_uvalid;		/* CHAR: following p_u* members from struct user are valid */
					/* XXX 64 bits for alignment */
	u_int64_t p_ustart_sec;		/* STRUCT TIMEVAL: starting time. */
	u_int32_t p_ustart_usec;	/* STRUCT TIMEVAL: starting time. */

	u_int32_t p_uutime_sec;		/* STRUCT TIMEVAL: user time. */
	u_int32_t p_uutime_usec;	/* STRUCT TIMEVAL: user time. */
	u_int32_t p_ustime_sec;		/* STRUCT TIMEVAL: system time. */
	u_int32_t p_ustime_usec;	/* STRUCT TIMEVAL: system time. */

	u_int64_t p_uru_maxrss;		/* LONG: max resident set size. */
	u_int64_t p_uru_ixrss;		/* LONG: integral shared memory size. */
	u_int64_t p_uru_idrss;		/* LONG: integral unshared data ". */
	u_int64_t p_uru_isrss;		/* LONG: integral unshared stack ". */
	u_int64_t p_uru_minflt;		/* LONG: page reclaims. */
	u_int64_t p_uru_majflt;		/* LONG: page faults. */
	u_int64_t p_uru_nswap;		/* LONG: swaps. */
	u_int64_t p_uru_inblock;	/* LONG: block input operations. */
	u_int64_t p_uru_oublock;	/* LONG: block output operations. */
	u_int64_t p_uru_msgsnd;		/* LONG: messages sent. */
	u_int64_t p_uru_msgrcv;		/* LONG: messages received. */
	u_int64_t p_uru_nsignals;	/* LONG: signals received. */
	u_int64_t p_uru_nvcsw;		/* LONG: voluntary context switches. */
	u_int64_t p_uru_nivcsw;		/* LONG: involuntary ". */

	u_int32_t p_uctime_sec;		/* STRUCT TIMEVAL: child u+s time. */
	u_int32_t p_uctime_usec;	/* STRUCT TIMEVAL: child u+s time. */
	int32_t p_psflags;		/* INT: PS_* flags on the process. */
	int32_t p_spare;		/* INT: unused. */
	u_int32_t p_svuid;		/* UID_T: saved user id */
	u_int32_t p_svgid;		/* GID_T: saved group id */
	char    p_emul[KI_EMULNAMELEN];	/* syscall emulation name */
	u_int64_t p_rlim_rss_cur;	/* RLIM_T: soft limit for rss */
	u_int64_t p_cpuid;		/* LONG: CPU id */
	u_int64_t p_vm_map_size;	/* VSIZE_T: virtual size */
	int32_t   p_tid;		/* PID_T: Thread identifier. */
	u_int32_t p_rtableid;		/* U_INT: Routing table identifier. */
};

/*
 * VM address range entry, matching struct vm_map_entry.  Useful for
 * debuggers to know process's addresses.
 *
 * To iterate entries, set the last kve_end as the base address into
 * kve_start.
 */
struct kinfo_vmentry {
	u_long kve_start;		/* vaddr_t */
	u_long kve_end;			/* vaddr_t */
	u_long kve_guard;		/* vsize_t */
	u_long kve_fspace;		/* vsize_t */
	u_long kve_fspace_augment;	/* vsize_t */
	u_int64_t kve_offset;		/* voff_t */
	int kve_wired_count;
	int kve_etype;
	int kve_protection;
	int kve_max_protection;
	int kve_advice;
	int kve_inheritance;
	u_int8_t kve_flags;		/* u_int8_t */
};

#define KVE_ET_OBJ		0x00000001
#define KVE_ET_SUBMAP		0x00000002
#define KVE_ET_COPYONWRITE 	0x00000004
#define KVE_ET_NEEDSCOPY	0x00000008
#define KVE_ET_HOLE		0x00000010
#define KVE_ET_NOFAULT		0x00000020
#define KVE_ET_FREEMAPPED	0x00000080
#define KVE_PROT_NONE		0x00000000
#define KVE_PROT_READ		0x00000001
#define KVE_PROT_WRITE		0x00000002
#define KVE_PROT_EXEC		0x00000004
#define KVE_ADV_NORMAL		0x00000000
#define KVE_ADV_RANDOM		0x00000001
#define KVE_ADV_SEQUENTIAL	0x00000002
#define KVE_INH_SHARE		0x00000000
#define KVE_INH_COPY		0x00000010
#define KVE_INH_NONE		0x00000020
#define KVE_INH_ZERO		0x00000030
#define KVE_F_STATIC		0x01
#define KVE_F_KMEM		0x02

/*
 * kern.file returns an array of these structures, which are designed
 * both to be immune to 32/64 bit emulation issues and to
 * provide backwards compatibility.  The order differs slightly from
 * that of the real struct file, and some fields are taken from other
 * structures (struct vnode, struct proc) in order to make the file
 * information more useful.
 */
#define	KERN_FILE_BYFILE	1
#define	KERN_FILE_BYPID		2
#define	KERN_FILE_BYUID		3
#define	KERN_FILESLOP		10

#define KERN_FILE_TEXT		-1
#define KERN_FILE_CDIR		-2
#define KERN_FILE_RDIR		-3
#define KERN_FILE_TRACE		-4

#define KI_MNAMELEN		96	/* rounded up from 90 */
#define KI_UNPPATHLEN		104

struct kinfo_file {
	uint64_t	f_fileaddr;	/* PTR: address of struct file */
	uint32_t	f_flag;		/* SHORT: flags (see fcntl.h) */
	uint32_t	f_iflags;	/* INT: internal flags */
	uint32_t	f_type;		/* INT: descriptor type */
	uint32_t	f_count;	/* UINT: reference count */
	uint32_t	f_msgcount;	/* UINT: references from msg queue */
	uint32_t	f_usecount;	/* INT: number active users */
	uint64_t	f_ucred;	/* PTR: creds for descriptor */
	uint32_t	f_uid;		/* UID_T: descriptor credentials */
	uint32_t	f_gid;		/* GID_T: descriptor credentials */
	uint64_t	f_ops;		/* PTR: address of fileops */
	uint64_t	f_offset;	/* OFF_T: offset */
	uint64_t	f_data;		/* PTR: descriptor data */
	uint64_t	f_rxfer;	/* UINT64: number of read xfers */
	uint64_t	f_rwfer;	/* UINT64: number of write xfers */
	uint64_t	f_seek;		/* UINT64: number of seek operations */
	uint64_t	f_rbytes;	/* UINT64: total bytes read */
	uint64_t	f_wbytes;	/* UINT64: total bytes written */

	/* information about the vnode associated with this file */
	uint64_t	v_un;		/* PTR: socket, specinfo, etc */
	uint32_t	v_type;		/* ENUM: vnode type */
	uint32_t	v_tag;		/* ENUM: type of underlying data */
	uint32_t	v_flag;		/* UINT: vnode flags */
	uint32_t	va_rdev;	/* DEV_T: raw device */
	uint64_t	v_data;		/* PTR: private data for fs */
	uint64_t	v_mount;	/* PTR: mount info for fs */
	uint64_t	va_fileid;	/* LONG: file id */
	uint64_t	va_size;	/* UINT64_T: file size in bytes */
	uint32_t	va_mode;	/* MODE_T: file access mode and type */
	uint32_t	va_fsid;	/* DEV_T: filesystem device */
	char		f_mntonname[KI_MNAMELEN];

	/* socket information */
	uint32_t	so_type;	/* SHORT: socket type */
	uint32_t	so_state;	/* SHORT: socket state */
	uint64_t	so_pcb;		/* PTR: socket pcb */
					/* for non-root: -1 if not NULL */
	uint32_t	so_protocol;	/* SHORT: socket protocol type */
	uint32_t	so_family;	/* INT: socket domain family */
	uint64_t	inp_ppcb;	/* PTR: pointer to per-protocol pcb */
	uint32_t	inp_lport;	/* SHORT: local inet port */
	uint32_t	inp_laddru[4];	/* STRUCT: local inet addr */
	uint32_t	inp_fport;	/* SHORT: foreign inet port */
	uint32_t	inp_faddru[4];	/* STRUCT: foreign inet addr */
	uint64_t	unp_conn;	/* PTR: connected socket cntrl block */

	/* pipe information */
	uint64_t	pipe_peer;	/* PTR: link with other direction */
	uint32_t	pipe_state;	/* UINT: pipe status info */

	/* kqueue information */
	uint32_t	kq_count;	/* INT: number of pending events */
	uint32_t	kq_state;	/* INT: kqueue status information */

	uint32_t	__unused1;	/* INT: unused */

	/* process information when retrieved via KERN_FILE_BY[PU]ID */
	uint32_t	p_pid;		/* PID_T: process id */
	int32_t		fd_fd;		/* INT: descriptor number */
	uint32_t	fd_ofileflags;	/* CHAR: open file flags */
	uint32_t	p_uid;		/* UID_T: process credentials */
	uint32_t	p_gid;		/* GID_T: process credentials */
	uint32_t	p_tid;		/* PID_T: thread id */
	char		p_comm[KI_MAXCOMLEN];

	/* more socket information */
	uint32_t	inp_rtableid;	/* UINT: Routing table identifier. */
	uint64_t	so_splice;	/* PTR: f_data of spliced socket */
	int64_t		so_splicelen;	/* OFF_T: already spliced count or */
					/* -1 if this is target of splice */
	uint64_t	so_rcv_cc;	/* LONG: chars in receive buf */
	uint64_t	so_snd_cc;	/* LONG: chars in send buf */
	uint64_t	unp_refs;	/* PTR: connected sockets */
	uint64_t	unp_nextref;	/* PTR: link to next connected socket */
	uint64_t	unp_addr;	/* PTR: address of the socket address */
	char		unp_path[KI_UNPPATHLEN];
	uint32_t	inp_proto;	/* CHAR: raw protocol id */
	uint32_t	t_state;	/* SHORT: tcp state */
	uint64_t	t_rcv_wnd;	/* ULONG: tcp receive window */
	uint64_t	t_snd_wnd;	/* ULONG: tcp send window */
	uint64_t	t_snd_cwnd;	/* ULONG: congestion-controlled win */

	uint32_t	va_nlink;	/* NLINK_T: number of references to file */
};

/*
 * CTL_FS identifiers
 */
#define	FS_POSIX	1		/* POSIX flags */
#define	FS_MAXID	2

#define	CTL_FS_NAMES { \
	{ 0, 0 }, \
	{ "posix", CTLTYPE_NODE }, \
}

/*
 * CTL_FS identifiers
 */
#define	FS_POSIX_SETUID	1		/* int: always clear SGID/SUID bit when owner change */
#define	FS_POSIX_MAXID	2

#define	CTL_FS_POSIX_NAMES { \
	{ 0, 0 }, \
	{ "setuid", CTLTYPE_INT }, \
}

/*
 * CTL_HW identifiers
 */
#define	HW_MACHINE		 1	/* string: machine class */
#define	HW_MODEL		 2	/* string: specific machine model */
#define	HW_NCPU			 3	/* int: number of cpus being used */
#define	HW_BYTEORDER		 4	/* int: machine byte order */
#define	HW_PHYSMEM		 5	/* int: total memory */
#define	HW_USERMEM		 6	/* int: non-kernel memory */
#define	HW_PAGESIZE		 7	/* int: software page size */
#define	HW_DISKNAMES		 8	/* strings: disk drive names */
#define	HW_DISKSTATS		 9	/* struct: diskstats[] */
#define	HW_DISKCOUNT		10	/* int: number of disks */
#define	HW_SENSORS		11	/* node: hardware monitors */
#define	HW_CPUSPEED		12	/* get CPU frequency */
#define	HW_SETPERF		13	/* set CPU performance % */
#define	HW_VENDOR		14	/* string: vendor name */
#define	HW_PRODUCT		15	/* string: product name */
#define	HW_VERSION		16	/* string: hardware version */
#define	HW_SERIALNO		17	/* string: hardware serial number */
#define	HW_UUID			18	/* string: universal unique id */
#define	HW_PHYSMEM64		19	/* quad: total memory */
#define	HW_USERMEM64		20	/* quad: non-kernel memory */
#define	HW_NCPUFOUND		21	/* int: number of cpus found*/
#define	HW_ALLOWPOWERDOWN	22	/* allow power button shutdown */
#define	HW_PERFPOLICY		23	/* set performance policy */
#define	HW_MAXID		24	/* number of valid hw ids */

#define	CTL_HW_NAMES { \
	{ 0, 0 }, \
	{ "machine", CTLTYPE_STRING }, \
	{ "model", CTLTYPE_STRING }, \
	{ "ncpu", CTLTYPE_INT }, \
	{ "byteorder", CTLTYPE_INT }, \
	{ "gap", 0 }, \
	{ "gap", 0 }, \
	{ "pagesize", CTLTYPE_INT }, \
	{ "disknames", CTLTYPE_STRING }, \
	{ "diskstats", CTLTYPE_STRUCT }, \
	{ "diskcount", CTLTYPE_INT }, \
	{ "sensors", CTLTYPE_NODE}, \
	{ "cpuspeed", CTLTYPE_INT }, \
	{ "setperf", CTLTYPE_INT }, \
	{ "vendor", CTLTYPE_STRING }, \
	{ "product", CTLTYPE_STRING }, \
	{ "version", CTLTYPE_STRING }, \
	{ "serialno", CTLTYPE_STRING }, \
	{ "uuid", CTLTYPE_STRING }, \
	{ "physmem", CTLTYPE_QUAD }, \
	{ "usermem", CTLTYPE_QUAD }, \
	{ "ncpufound", CTLTYPE_INT }, \
	{ "allowpowerdown", CTLTYPE_INT }, \
	{ "perfpolicy", CTLTYPE_STRING }, \
}

/*
 * CTL_DEBUG definitions
 *
 * Second level identifier specifies which debug variable.
 * Third level identifier specifies which structure component.
 */
#define	CTL_DEBUG_NAME		0	/* string: variable name */
#define	CTL_DEBUG_VALUE		1	/* int: variable value */
#define	CTL_DEBUG_MAXID		20

#endif	/* !_COMPAT_SYS_SYSCTL_H_ */
