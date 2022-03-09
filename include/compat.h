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

#ifndef _COMPAT_H_
#define _COMPAT_H_

#ifdef _AIX
#include <sys/limits.h>
#endif /* ifdef _AIX */

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif /* ifndef _GNU_SOURCE */

#if !defined(DEF_STRONG)
#define DEF_STRONG(x)
#endif /* if !defined( DEF_STRONG ) */

#if !defined(PROTO_NORMAL)
#define PROTO_NORMAL(x)
#endif /* if !defined( PROTO_NORMAL ) */

#if !defined(DEF_WEAK)
#define DEF_WEAK(x)
#endif /* if !defined( DEF_WEAK ) */

#ifndef __OpenBSD__

#ifdef FAIL_INSTEAD_OF_TRYING_FALLBACK
#define FAIL_INSTEAD_OF_TRYING_FALLBACK
#endif /* ifndef FAIL_INSTEAD_OF_TRYING_FALLBACK */

#if !defined(HAVE_ATTRIBUTE__BOUNDED__)
#define __bounded__(x, y, z)
#endif /* if !defined( HAVE_ATTRIBUTE__BOUNDED__ ) */

#ifndef __warn_references
#define __warn_references(x, y)
#endif /* ifndef __warn_references */

#ifndef __UNUSED
#define __UNUSED __attribute__((unused))
#endif /* ifndef __UNUSED */

#ifndef __dead
#define __dead __attribute__((__noreturn__))
#endif /* ifndef __dead */

#ifndef __BEGIN_DECLS
#define __BEGIN_DECLS
#endif /* ifndef __BEGIN_DECLS */

#ifndef __END_DECLS
#define __END_DECLS
#endif /* ifndef __END_DECLS */

#ifndef __BEGIN_HIDDEN_DECLS
#define __BEGIN_HIDDEN_DECLS
#endif /* ifndef __BEGIN_HIDDEN_DECLS */

#ifndef __END_HIDDEN_DECLS
#define __END_HIDDEN_DECLS
#endif /* ifndef __END_HIDDEN_DECLS */

#ifndef __CONCAT
#define __CONCAT(x, y) x##y
#endif /* ifndef __CONCAT */
#ifndef __STRING
#define __STRING(x) #x
#endif /* ifndef __STRING */

#ifndef __NetBSD__
#undef __weak_alias
#define __weak_alias(new, old)                                                 \
  extern __typeof(old) new __attribute__((weak, alias(#old)))

#define __strong_alias(new, old)                                               \
  extern __typeof(old) new __attribute__((alias(#old)))
#endif /* ifndef __NetBSD__ */

#ifndef SA_LEN
#define SA_LEN(X)                                                              \
  (((struct sockaddr *)(X))->sa_family == AF_INET ? sizeof(struct sockaddr_in) \
   : ((struct sockaddr *)(X))->sa_family == AF_INET6                           \
       ? sizeof(struct sockaddr_in6)                                           \
       : sizeof(struct sockaddr))
#endif /* ifndef SA_LEN */

#ifndef SS_LEN
#define SS_LEN(X)                                                              \
  (((struct sockaddr_storage *)(X))->ss_family == AF_INET                      \
       ? sizeof(struct sockaddr_in)                                            \
   : ((struct sockaddr_storage *)(X))->ss_family == AF_INET6                   \
       ? sizeof(struct sockaddr_in6)                                           \
       : sizeof(struct sockaddr))
#endif /* ifndef SS_LEN */

#define _PW_BUF_LEN 1024
#define _GR_BUF_LEN 1024

#define NOFILE_MAX NOFILE

/* sys/sys/param.h */
/*
 * File system parameters and macros.
 *
 * The file system is made out of blocks of at most MAXBSIZE units, with
 * smaller units (fragments) only in the last direct block.  MAXBSIZE
 * primarily determines the size of buffers in the buffer pool.  It may be
 * made larger without any effect on existing file systems; however making
 * it smaller makes some file systems unmountable.
 */
#define MAXBSIZE (64 * 1024)

#ifndef NL_TEXTMAX
#define NL_TEXTMAX 2048
#endif /* ifndef NL_TEXTMAX */

#ifndef howmany
#define howmany(n, d) (((n) + ((d)-1)) / (d))
#endif /* ifndef howmany */

/* pwd.h */
#define _PW_NAME_LEN 63

/* unistd.h */
#ifndef HAVE_EXECVPE
int execvpe(const char *, char *const *, char *const *);
#endif /* !HAVE_EXECVPE */

#ifndef HAVE_SETPROGNAME
const char *getprogname(void);
void setprogname(const char *progname);
#endif /* !HAVE_SETPROGNAME */

/* getbsize.c */
char *getbsize(int *, long *);

/* #define d_namlen d_reclen */
#if !defined(_DIRENT_HAVE_NAMLEN)
#define D_NAMLEN(x) strnlen((x)->d_name, (x)->d_reclen)
#else /* if !defined( _DIRENT_HAVE_NAMLEN ) */
#define D_NAMLEN(x) (x)->d_namlen
#endif /* if !defined( _DIRENT_HAVE_NAMLEN ) */

#ifndef S_BLKSIZE
#define S_BLKSIZE 512
#endif /* ifndef S_BLKSIZE */

#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif /* ifndef MAXNAMLEN */

#ifndef UID_MAX
#define UID_MAX 60000
#endif /* ifndef UID_MAX */

#ifndef GID_MAX
#define GID_MAX 60000
#endif /* ifndef GID_MAX */

/* sys/sys/stat.h */
#ifndef ACCESSPERMS
#define ACCESSPERMS (S_IRWXU | S_IRWXG | S_IRWXO) /* 00777 */
#endif                                            /* ifndef ACCESSPERMS */

#ifndef ALLPERMS
#define ALLPERMS                                                               \
  (S_ISUID | S_ISGID | S_ISTXT | S_IRWXU | S_IRWXG | S_IRWXO) /* 00666 */
#endif /* ifndef ALLPERMS */

#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#endif /* ifndef DEFFILEMODE */

#ifdef S_ISVTX
#ifndef S_ISTXT
#define S_ISTXT S_ISVTX
#endif /* ifndef S_ISVTX */
#endif /* ifdef S_ISVTX */

/* lib/libc/include/thread_private.h */
#define _MUTEX_LOCK(mutex)                                                     \
  do {                                                                         \
  } while (0)
#define _MUTEX_UNLOCK(mutex)                                                   \
  do {                                                                         \
  } while (0)

#define SHA512_Update SHA512Update
#define SHA512_CTX SHA2_CTX
#define SHA512_Init SHA512Init
#define SHA512_Final SHA512Final

/* sys/socket.h */
#ifndef RT_TABLEID_MAX
#define RT_TABLEID_MAX 255
#endif /* ifndef RT_TABLEID_MAX */

/* sys/syslimits.h */
#ifndef CHILD_MAX
#define CHILD_MAX 80 /* max simultaneous processes */
#endif               /* ifndef CHILD_MAX */

/* setproctitle.c */
void setproctitle(const char *, ...);

/* pw_dup.c */
struct passwd *pw_dup(const struct passwd *);

int issetugid(void);

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif /* ifndef HOST_NAME_MAX */

#ifndef OPEN_MAX
#define OPEN_MAX 64 /* max open files per process */
#endif              /* ifndef OPEN_MAX */

#endif /* ifndef __OpenBSD__ */

/* pledge */
int openbsd_pledge(const char *, const char *);

#endif /* ifndef _COMPAT_H_ */
