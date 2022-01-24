#include <sys/vfs.h>
#include_next <sys/mount.h>

#ifndef _COMPAT_SYS_MOUNT_H_
#define _COMPAT_SYS_MOUNT_H_

#include <stdlib.h>

/*
 * Flags for various system call interfaces.
 *
 * waitfor flags to vfs_sync() and getfsstat()
 */
#define	MNT_NOWAIT	1
#define	MNT_WAIT	2
#define	MNT_LAZY	3

/*
 * Flags set by internal operations.
 */
#define	MNT_LOCAL	0x00001000	/* filesystem is stored locally */
#define	MNT_QUOTA	0x00002000	/* quotas are enabled on filesystem */
#define	MNT_ROOTFS	0x00004000	/* identifies the root filesystem */

/*
 * file system statistics
 */

#define	MFSNAMELEN	16	/* length of fs type name, including nul */
#define	MNAMELEN	90	/* length of buffer for returned name */

/*
 * Mount flags.
 *
 * Unmount uses MNT_FORCE flag.
 */
#define	MNT_RDONLY	0x00000001	/* read only filesystem */
#define	MNT_SYNCHRONOUS	0x00000002	/* file system written synchronously */
#define	MNT_NOEXEC	0x00000004	/* can't exec from filesystem */
#define	MNT_NOSUID	0x00000008	/* don't honor setuid bits on fs */
#define	MNT_NODEV	0x00000010	/* don't interpret special files */
#define	MNT_NOPERM	0x00000020	/* don't enforce permission checks */
#define	MNT_ASYNC	0x00000040	/* file system written asynchronously */
#define	MNT_WXALLOWED	0x00000800	/* filesystem allows W|X mappings */

/*
 * File system types.
 */
#define	MOUNT_FFS	"ffs"		/* UNIX "Fast" Filesystem */
#define	MOUNT_UFS	MOUNT_FFS	/* for compatibility */
#define	MOUNT_NFS	"nfs"		/* Network Filesystem */
#define	MOUNT_MFS	"mfs"		/* Memory Filesystem */
#define	MOUNT_MSDOS	"msdos"		/* MSDOS Filesystem */
#define	MOUNT_AFS	"afs"		/* Andrew Filesystem */
#define	MOUNT_CD9660	"cd9660"	/* ISO9660 (aka CDROM) Filesystem */
#define	MOUNT_EXT2FS	"ext2fs"	/* Second Extended Filesystem */
#define	MOUNT_NCPFS	"ncpfs"		/* NetWare Network File System */
#define	MOUNT_NTFS	"ntfs"		/* NTFS */
#define	MOUNT_UDF	"udf"		/* UDF */
#define	MOUNT_TMPFS	"tmpfs"		/* tmpfs */
#define	MOUNT_FUSEFS	"fuse"		/* FUSE */

union mount_info {

};

struct openbsd_statfs {
	struct statfs buf;
	dev_t st_dev;
	char f_fstypename[MFSNAMELEN];  /* fs type name */
	char f_mntonname[MNAMELEN];     /* directory on which mounted */
	char f_mntfromname[MNAMELEN];   /* mounted file system */
	char f_mntfromspec[MNAMELEN];   /* special for mount request */
	union mount_info mount_info;    /* per-filesystem mount options */
};

#ifdef	__NEED_OPENBSD_statfs
#define	statfs		openbsd_statfs

#define	f_blocks	buf.f_blocks
#define	f_bsize		buf.f_bsize
#define	f_bfree		buf.f_bfree
#define	f_bavail	buf.f_bavail

#define	f_files		buf.f_files
#define	f_ffree		buf.f_ffree
#define	f_flags		buf.f_flags
#endif	/* !__NEED_OPENBSD_statfs */

int getmntinfo(struct openbsd_statfs **, int);
int getfsstat(struct openbsd_statfs *, size_t, int);
int openbsd_statfs(const char *, struct openbsd_statfs *);

#endif /* _COMPAT_SYS_MOUNT_H_ */
