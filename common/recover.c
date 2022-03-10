/*      $OpenBSD: recover.c,v 1.32 2022/02/20 19:45:51 tb Exp $    */

/*-
 * Copyright (c) 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include "../include/compat.h"

#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>

/*
 * We include <sys/file.h>, because the open #defines were found there
 * on historical systems.  We also include <bsd_fcntl.h> because the open(2)
 * #defines are found there on newer systems.
 */
#include <sys/file.h>

#include <stddef.h>
#include <stdint.h>
#include <bitstring.h>
#ifdef __solaris__
# define _XPG7
#endif /* ifdef __solaris__ */
#include <dirent.h>
#include <errno.h>
#include <bsd_fcntl.h>
#include <limits.h>
#include <paths.h>
#include <pwd.h>
#include <stdio.h>
#include <bsd_err.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <time.h>
#include <bsd_unistd.h>

#include "errc.h"
#include "common.h"
#include "pathnames.h"

#undef open

/*
 * Recovery code.
 *
 * The basic scheme is as follows:  In the EXF structure, we maintain full
 * paths of a b+tree file and a mail recovery file.  The former is the file
 * used as backing store by the DB package.  The latter is the file that
 * contains an email message to be sent to the user if we crash.  The two
 * simple states of recovery are:
 *
 *      + first starting the edit session:
 *              the b+tree file exists and is mode 700, the mail recovery
 *              file doesn't exist.
 *      + after the file has been modified:
 *              the b+tree file exists and is mode 600, the mail recovery
 *              file exists, and is exclusively locked.
 *
 * In the EXF structure we maintain a file descriptor that is the locked
 * file descriptor for the mail recovery file.  NOTE: we sometimes have to
 * do locking with fcntl(2).  This is a problem because if you close(2) any
 * file descriptor associated with the file, ALL of the locks go away.  Be
 * sure to remember that if you have to modify the recovery code.  (It has
 * been rhetorically asked of what the designers could have been thinking
 * when they did that interface.  The answer is simple: they weren't.)
 *
 * To find out if a recovery file/backing file pair are in use, try to get
 * a lock on the recovery file.
 *
 * To find out if a backing file can be deleted at boot time, check for an
 * owner execute bit.  (Yes, I know it's ugly, but it's either that or put
 * special stuff into the backing file itself, or correlate the files at
 * boot time, neither of which looks like fun.)  Note also that there's a
 * window between when the file is created and the X bit is set.  It's small,
 * but it's there.  To fix the window, check for 0 length files as well.
 *
 * To find out if a file can be recovered, check the F_RCV_ON bit.  Note,
 * this DOES NOT mean that any initialization has been done, only that we
 * haven't yet failed at setting up or doing recovery.
 *
 * To preserve a recovery file/backing file pair, set the F_RCV_NORM bit.
 * If that bit is not set when ending a file session:
 *      If the EXF structure paths (rcv_path and rcv_mpath) are not NULL,
 *      they are unlink(2)'d, and free(3)'d.
 *      If the EXF file descriptor (rcv_fd) is not -1, it is closed.
 *
 * The backing b+tree file is set up when a file is first edited, so that
 * the DB package can use it for on-disk caching and/or to snapshot the
 * file.  When the file is first modified, the mail recovery file is created,
 * the backing file permissions are updated, the file is sync(2)'d to disk,
 * and the timer is started.  Then, at RCV_PERIOD second intervals, the
 * b+tree file is synced to disk.  RCV_PERIOD is measured using SIGALRM, which
 * means that the data structures (SCR, EXF, the underlying tree structures)
 * must be consistent when the signal arrives.
 *
 * The recovery mail file contains normal mail headers, with two additions,
 * which occur in THIS order, as the FIRST TWO headers:
 *
 *      X-vi-recover-file: file_name
 *      X-vi-recover-path: recover_path
 *
 * Since newlines delimit the headers, this means that file names cannot have
 * newlines in them, but that's probably okay.  As these files aren't intended
 * to be long-lived, changing their format won't be too painful.
 *
 * Btree files are named "vi.XXXX" and recovery files are named "recover.XXXX".
 */

#define VI_FHEADER      "X-vi-recover-file: "
#define VI_PHEADER      "X-vi-recover-path: "

int rcv_copy(SCR *, int, char *);
void rcv_email(SCR *, int);
int rcv_mailfile(SCR *, int, char *);
char *rcv_gets(char *, size_t, int);

int      rcv_mktemp(SCR *, char *, char *, int);
int      rcv_openat(SCR *, int, const char *, int *);

/*
 * rcv_tmp --
 *      Build a file name that will be used as the recovery file.
 *
 * PUBLIC: int rcv_tmp(SCR *, EXF *, char *);
 */
int
rcv_tmp(SCR *sp, EXF *ep, char *name)
{
        struct stat sb;
        static int warned = 0;
        int fd;
        char *dp, *p, path[PATH_MAX];

        /*
         * !!!
         * ep MAY NOT BE THE SAME AS sp->ep, DON'T USE THE LATTER.
         */
        if (opts_empty(sp, O_RECDIR, 0))
                goto err;
        dp = O_STR(sp, O_RECDIR);
        if (stat(dp, &sb)) {
                if (!warned) {
                        warned = 1;
                        msgq(sp, M_SYSERR, "%s", dp);
                        goto err;
                }
                return 1;
        }

        /* Newlines delimit the mail messages. */
        for (p = name; *p; ++p)
                if (*p == '\n') {
                        msgq(sp, M_ERR,
                    "Files with newlines in the name are unrecoverable");
                        goto err;
                }

        (void)snprintf(path, sizeof(path), "%s/vi.XXXXXX", dp);
        if ((fd = rcv_mktemp(sp, path, dp, S_IRWXU)) == -1)
                goto err;
        (void)close(fd);

        if ((ep->rcv_path = strdup(path)) == NULL) {
                msgq(sp, M_SYSERR, NULL);
                (void)unlink(path);
err:            msgq(sp, M_ERR,
                    "Modifications not recoverable if the session fails");
                return (1);
        }

        /* We believe the file is recoverable. */
        F_SET(ep, F_RCV_ON);
        return (0);
}

/*
 * rcv_init --
 *      Force the file to be snapshotted for recovery.
 *
 * PUBLIC: int rcv_init(SCR *);
 */
int
rcv_init(SCR *sp)
{
        EXF *ep;
        recno_t lno;

        ep = sp->ep;

        /* Only do this once. */
        F_CLR(ep, F_FIRSTMODIFY);

        /* If we already know the file isn't recoverable, we're done. */
        if (!F_ISSET(ep, F_RCV_ON))
                return (0);

        /* Turn off recoverability until we figure out if this will work. */
        F_CLR(ep, F_RCV_ON);

        /* Test if we're recovering a file, not editing one. */
        if (ep->rcv_mpath == NULL) {
                /* Build a file to mail to the user. */
                if (rcv_mailfile(sp, 0, NULL))
                        goto err;

                /* Force a read of the entire file. */
                if (db_last(sp, &lno))
                        goto err;

                /* Turn on a busy message, and sync it to backing store. */
                sp->gp->scr_busy(sp,
                    "Copying file for recovery...", BUSY_ON);
                if (ep->db->sync(ep->db, R_RECNOSYNC)) {
                        msgq_str(sp, M_SYSERR, ep->rcv_path,
                            "Preservation failed: %s");
                        sp->gp->scr_busy(sp, NULL, BUSY_OFF);
                        goto err;
                }
                sp->gp->scr_busy(sp, NULL, BUSY_OFF);
        }

        /* Turn off the owner execute bit. */
        (void)chmod(ep->rcv_path, S_IRUSR | S_IWUSR);

        /* We believe the file is recoverable. */
        F_SET(ep, F_RCV_ON);
        return (0);

err:    msgq(sp, M_ERR,
            "Modifications not recoverable if the session fails");
        return (1);
}

/*
 * rcv_sync --
 *      Sync the file, optionally:
 *              flagging the backup file to be preserved
 *              snapshotting the backup file and send email to the user
 *              sending email to the user if the file was modified
 *              ending the file session
 *
 * PUBLIC: int rcv_sync(SCR *, unsigned int);
 */
int
rcv_sync(SCR *sp, unsigned int flags)
{
        EXF *ep;
        int fd, rval;
        char *dp, buf[1024];

        /* Make sure that there's something to recover/sync. */
        ep = sp->ep;
        if (ep == NULL || !F_ISSET(ep, F_RCV_ON))
                return (0);

        /* Sync the file if it's been modified. */
        if (F_ISSET(ep, F_MODIFIED)) {
                /* Clear recovery sync flag. */
                F_CLR(ep, F_RCV_SYNC);
                if (ep->db->sync(ep->db, R_RECNOSYNC)) {
                        F_CLR(ep, F_RCV_ON | F_RCV_NORM);
                        msgq_str(sp, M_SYSERR,
                            ep->rcv_path, "File backup failed: %s");
                        return (1);
                }

                /* REQUEST: don't remove backing file on exit. */
                if (LF_ISSET(RCV_PRESERVE))
                        F_SET(ep, F_RCV_NORM);

                /* REQUEST: send email. */
                if (LF_ISSET(RCV_EMAIL))
                        rcv_email(sp, ep->rcv_fd);
        }

        /*
         * !!!
         * Each time the user exec's :preserve, we have to snapshot all of
         * the recovery information, i.e. it's like the user re-edited the
         * file.  We copy the DB(3) backing file, and then create a new mail
         * recovery file, it's simpler than exiting and reopening all of the
         * underlying files.
         *
         * REQUEST: snapshot the file.
         */
        rval = 0;
        if (LF_ISSET(RCV_SNAPSHOT)) {
                if (opts_empty(sp, O_RECDIR, 0))
                        goto err;
                dp = O_STR(sp, O_RECDIR);
                (void)snprintf(buf, sizeof(buf), "%s/vi.XXXXXX", dp);
                if ((fd = rcv_mktemp(sp, buf, dp, S_IRUSR | S_IWUSR)) == -1)
                        goto err;
                sp->gp->scr_busy(sp,
                    "Copying file for recovery...", BUSY_ON);
                if (rcv_copy(sp, fd, ep->rcv_path) ||
                    close(fd) || rcv_mailfile(sp, 1, buf)) {
                        (void)unlink(buf);
                        (void)close(fd);
                        rval = 1;
                }
                sp->gp->scr_busy(sp, NULL, BUSY_OFF);
        }
        if (0) {
err:            rval = 1;
        }

        /* REQUEST: end the file session. */
        if (LF_ISSET(RCV_ENDSESSION))
                F_SET(sp, SC_EXIT_FORCE);
        return (rval);
}

/*
 * rcv_mailfile --
 *      Build the file to mail to the user.
 */
int
rcv_mailfile(SCR *sp, int issync, char *cp_path)
{
        EXF *ep;
        GS *gp;
        struct passwd *pw;
        size_t len;
        time_t now;
        uid_t uid;
        int fd;
        char *dp, *p, *t, buf[4096], mpath[PATH_MAX];
        char *t1, *t2, *t3;
        char host[HOST_NAME_MAX+1];

        gp = sp->gp;
        (void)gp;
        if ((pw = getpwuid(uid = getuid())) == NULL) {
                msgq(sp, M_ERR,
                    "Information on user id %u not found", uid);
                return (1);
        }

        if (opts_empty(sp, O_RECDIR, 0))
                return (1);
        dp = O_STR(sp, O_RECDIR);
        (void)snprintf(mpath, sizeof(mpath), "%s/recover.XXXXXX", dp);
        if ((fd = rcv_mktemp(sp, mpath, dp, S_IRUSR | S_IWUSR)) == -1)
                return (1);

        /*
         * XXX
         * We keep an open lock on the file so that the recover option can
         * distinguish between files that are live and those that need to
         * be recovered.  There's an obvious window between the mkstemp call
         * and the lock, but it's pretty small.
         */
        ep = sp->ep;
        if (file_lock(sp, NULL, NULL, fd, 1) != LOCK_SUCCESS)
                msgq(sp, M_SYSERR, "Unable to lock recovery file");
        if (!issync) {
                /* Save the recover file descriptor, and mail path. */
                ep->rcv_fd = fd;
                if ((ep->rcv_mpath = strdup(mpath)) == NULL) {
                        msgq(sp, M_SYSERR, NULL);
                        goto err;
                }
                cp_path = ep->rcv_path;
        }

        /*
         * XXX
         * We can't use stdio(3) here.  The problem is that we may be using
         * fcntl(2), so if ANY file descriptor into the file is closed, the
         * lock is lost.  So, we could never close the FILE *, even if we
         * dup'd the fd first.
         */
        t = sp->frp->name;
        if ((p = strrchr(t, '/')) == NULL)
                p = t;
        else
                ++p;
        (void)time(&now);
        (void)gethostname(host, sizeof(host));
        len = snprintf(buf, sizeof(buf),
            "%s%s\n%s%s\n%s\n%s\n%s%s\n%s%s\n%s\n%s\n\n",
            VI_FHEADER, t,                      /* Non-standard. */
            VI_PHEADER, cp_path,                /* Non-standard. */
            "Reply-To: root",
            "From: root (OpenVi recovery program)",
            "To: ", pw->pw_name,
            "Subject: OpenVi saved the file ", p,
            "Precedence: bulk",                 /* For vacation(1). */
            "Auto-Submitted: auto-generated");
        if (len > sizeof(buf) - 1)
                goto lerr;
        if (write(fd, buf, len) != len)
                goto werr;

        len = snprintf(buf, sizeof(buf),
            "%s%.24s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n\n",
            "On ", ctime(&now), ", the user ", pw->pw_name,
            " was editing a file named ", t, " on the machine ",
            host, ", when it was saved for recovery. ",
            "You can recover most, if not all, of the changes ",
            "to this file using the -r option to ", bsd_getprogname(), ":\n\n\t",
            bsd_getprogname(), " -r ", t);
        if (len > sizeof(buf) - 1) {
lerr:           msgq(sp, M_ERR, "Recovery file buffer overrun");
                goto err;
        }

        /*
         * Format the message.  (Yes, I know it's silly.)
         * Requires that the message end in a <newline>.
         */
#define FMTCOLS 60
        for (t1 = buf; len > 0; len -= t2 - t1, t1 = t2) {
                /* Check for a short length. */
                if (len <= FMTCOLS) {
                        t2 = t1 + (len - 1);
                        goto wout;
                }

                /* Check for a required <newline>. */
                t2 = strchr(t1, '\n');
                if (t2 - t1 <= FMTCOLS)
                        goto wout;

                /* Find the closest space, if any. */
                for (t3 = t2; t2 > t1; --t2)
                        if (*t2 == ' ') {
                                if (t2 - t1 <= FMTCOLS)
                                        goto wout;
                                t3 = t2;
                        }
                t2 = t3;

                /* t2 points to the last character to display. */
wout:           *t2++ = '\n';

                /* t2 points one after the last character to display. */
                if (write(fd, t1, t2 - t1) != t2 - t1)
                        goto werr;
        }

        if (issync) {
                rcv_email(sp, fd);
                if (close(fd)) {
werr:                   msgq(sp, M_SYSERR, "Recovery file");
                        goto err;
                }
        }
        return (0);

err:    if (!issync)
                ep->rcv_fd = -1;
        if (fd != -1)
                (void)close(fd);
        return (1);
}

/*
 * rcv_openat --
 *      Open a recovery file in the specified dir and lock it.
 *
 * PUBLIC: int rcv_openat(SCR *, int, const char *, int *)
 */
int
rcv_openat(SCR *sp, int dfd, const char *name, int *locked)
{
        struct stat sb;
        int fd, dummy;

        /*
         * If it's readable, it's recoverable.
         * Note: file_lock() sets the close on exec flag for us.
         */
        fd = openat(dfd, name, O_RDONLY|O_NOFOLLOW|O_NONBLOCK);
        if (fd == -1)
                goto bad;

        /*
         * Real vi recovery files are created with mode 0600.
         * If not a regular file or the mode has changed, skip it.
         */
        if (fstat(fd, &sb) == -1 || !S_ISREG(sb.st_mode) ||
            (sb.st_mode & ALLPERMS) != (S_IRUSR | S_IWUSR))
                goto bad;

        if (locked == NULL)
                locked = &dummy;
        switch ((*locked = file_lock(sp, NULL, NULL, fd, 0))) {
        case LOCK_FAILED:
                /*
                 * XXX
                 * Assume that a lock can't be acquired, but that we
                 * should permit recovery anyway.  If this is wrong,
                 * and someone else is using the file, we're going to
                 * die horribly.
                 */
                break;
        case LOCK_SUCCESS:
                break;
        case LOCK_UNAVAIL:
                /* If it's locked, it's live. */
                goto bad;
        }
        return fd;
bad:
        if (fd != -1)
                close(fd);
        return -1;
}

/*
 *      people making love
 *      never exactly the same
 *      just like a snowflake
 *
 * rcv_list --
 *      List the files that can be recovered by this user.
 *
 * PUBLIC: int rcv_list(SCR *);
 */
int
rcv_list(SCR *sp)
{
        struct dirent *dp;
        struct stat sb;
        DIR *dirp;
        int fd;
        FILE *fp;
        int found;
        char *p, *t, file[PATH_MAX], path[PATH_MAX];

        /* Open the recovery directory for reading. */
        if (opts_empty(sp, O_RECDIR, 0))
                return (1);
        p = O_STR(sp, O_RECDIR);
        if ((dirp = opendir(p)) == NULL) {
                msgq_str(sp, M_SYSERR, p, "recdir: %s");
                return (1);
        }

        /* Read the directory. */
        for (found = 0; (dp = readdir(dirp)) != NULL;) {
                if (strncmp(dp->d_name, "recover.", 8))
                        continue;

                if ((fd = rcv_openat(sp, dirfd(dirp), dp->d_name, NULL)) == -1)
                        continue;

                /* Check the headers. */
                if ((fp = fdopen(fd, "r")) == NULL) {
                        close(fd);
                        continue;
                }
                if (fgets(file, sizeof(file), fp) == NULL ||
                    strncmp(file, VI_FHEADER, sizeof(VI_FHEADER) - 1) ||
                    (p = strchr(file, '\n')) == NULL ||
                    fgets(path, sizeof(path), fp) == NULL ||
                    strncmp(path, VI_PHEADER, sizeof(VI_PHEADER) - 1) ||
                    (t = strchr(path, '\n')) == NULL) {
                        msgq_str(sp, M_ERR, dp->d_name,
                            "%s: malformed recovery file");
                        goto next;
                }
                *p = *t = '\0';

                /*
                 * If the file doesn't exist, it's an orphaned recovery file,
                 * toss it.
                 *
                 * XXX
                 * This can occur if the backup file was deleted and we crashed
                 * before deleting the email file.
                 */
                errno = 0;
                if (stat(path + sizeof(VI_PHEADER) - 1, &sb) &&
                    errno == ENOENT) {
                        (void)unlinkat(dirfd(dirp), dp->d_name, 0);
                        goto next;
                }

                /* Get the last modification time and display. */
                (void)fstat(fd, &sb);
                (void)printf("%.24s: %s\n",
                    ctime(&sb.st_mtime), file + sizeof(VI_FHEADER) - 1);
                found = 1;

                /* Close, discarding lock. */
next:           (void)fclose(fp);
        }
        if (found == 0)
                (void)printf("%s: No files to recover\n", bsd_getprogname());
        (void)closedir(dirp);
        return (0);
}

/*
 * rcv_read --
 *      Start a recovered file as the file to edit.
 *
 * PUBLIC: int rcv_read(SCR *, FREF *);
 */
int
rcv_read(SCR *sp, FREF *frp)
{
        struct dirent *dp;
        struct stat sb;
        DIR *dirp;
        EXF *ep;
#ifdef _AIX
        struct st_timespec rec_mtim;
#else
        struct timespec rec_mtim;
#endif /* ifdef _AIX */
        int fd, found, lck, requested, sv_fd;
        char *name, *p, *t, *rp, *recp, *pathp;
        char file[PATH_MAX], path[PATH_MAX], recpath[PATH_MAX];

        if (opts_empty(sp, O_RECDIR, 0))
                return (1);
        rp = O_STR(sp, O_RECDIR);
        if ((dirp = opendir(rp)) == NULL) {
                msgq_str(sp, M_SYSERR, rp, "%s");
                return (1);
        }

        name = frp->name;
        sv_fd = -1;
        rec_mtim.tv_sec = rec_mtim.tv_nsec = 0;
        recp = pathp = NULL;
        for (found = requested = 0; (dp = readdir(dirp)) != NULL;) {
                if (strncmp(dp->d_name, "recover.", 8))
                        continue;
                if ((size_t)snprintf(recpath, sizeof(recpath), "%s/%s",
                    rp, dp->d_name) >= sizeof(recpath))
                        continue;

                if ((fd = rcv_openat(sp, dirfd(dirp), dp->d_name, &lck)) == -1)
                        continue;

                /* Check the headers. */
                if (rcv_gets(file, sizeof(file), fd) == NULL ||
                    strncmp(file, VI_FHEADER, sizeof(VI_FHEADER) - 1) ||
                    (p = strchr(file, '\n')) == NULL ||
                    rcv_gets(path, sizeof(path), fd) == NULL ||
                    strncmp(path, VI_PHEADER, sizeof(VI_PHEADER) - 1) ||
                    (t = strchr(path, '\n')) == NULL) {
                        msgq_str(sp, M_ERR, recpath,
                            "%s: malformed recovery file");
                        goto next;
                }
                *p = *t = '\0';
                ++found;

                /*
                 * If the file doesn't exist, it's an orphaned recovery file,
                 * toss it.
                 *
                 * XXX
                 * This can occur if the backup file was deleted and we crashed
                 * before deleting the email file.
                 */
                errno = 0;
                if (stat(path + sizeof(VI_PHEADER) - 1, &sb) &&
                    errno == ENOENT) {
                        (void)unlink(dp->d_name);
                        goto next;
                }

                /* Check the file name. */
                if (strcmp(file + sizeof(VI_FHEADER) - 1, name))
                        goto next;

                ++requested;

                /*
                 * If we've found more than one, take the most recent.
                 */
                (void)fstat(fd, &sb);
                if (recp == NULL ||
                    timespeccmp(&rec_mtim, &sb.st_mtim, <)) {
                        p = recp;
                        t = pathp;
                        if ((recp = strdup(recpath)) == NULL) {
                                msgq(sp, M_SYSERR, NULL);
                                recp = p;
                                goto next;
                        }
                        if ((pathp = strdup(path)) == NULL) {
                                msgq(sp, M_SYSERR, NULL);
                                free(recp);
                                recp = p;
                                pathp = t;
                                goto next;
                        }
                        if (p != NULL) {
                                free(p);
                                free(t);
                        }
                        rec_mtim = sb.st_mtim;
                        if (sv_fd != -1)
                                (void)close(sv_fd);
                        sv_fd = fd;
                } else
next:                   (void)close(fd);
        }
        (void)closedir(dirp);

        if (recp == NULL) {
                msgq_str(sp, M_INFO, name,
                    "No files named %s, readable by you, to recover");
                return (1);
        }
        if (found) {
                if (requested > 1)
                        msgq(sp, M_INFO,
            "There are older versions of this file for you to recover");
                if (found > requested)
                        msgq(sp, M_INFO,
                            "There are other files for you to recover");
        }

        /*
         * Create the FREF structure, start the btree file.
         *
         * XXX
         * file_init() is going to set ep->rcv_path.
         */
        if (file_init(sp, frp, pathp + sizeof(VI_PHEADER) - 1, 0)) {
                free(recp);
                free(pathp);
                (void)close(sv_fd);
                return (1);
        }

        /*
         * We keep an open lock on the file so that the recover option can
         * distinguish between files that are live and those that need to
         * be recovered.  The lock is already acquired, just copy it.
         */
        ep = sp->ep;
        ep->rcv_mpath = recp;
        ep->rcv_fd = sv_fd;
        if (lck != LOCK_SUCCESS)
                F_SET(frp, FR_UNLOCKED);

        /* We believe the file is recoverable. */
        F_SET(ep, F_RCV_ON);
        return (0);
}

/*
 * rcv_copy --
 *      Copy a recovery file.
 */
int
rcv_copy(SCR *sp, int wfd, char *fname)
{
        int nr, nw, off, rfd;
        char buf[8 * 1024];

        if ((rfd = open(fname, O_RDONLY)) == -1)
                goto err;
        while ((nr = read(rfd, buf, sizeof(buf))) > 0)
                for (off = 0; nr; nr -= nw, off += nw)
                        if ((nw = write(wfd, buf + off, nr)) < 0)
                                goto err;
        if (nr == 0)
                return (0);

err:    msgq_str(sp, M_SYSERR, fname, "%s");
        return (1);
}

/*
 * rcv_gets --
 *      Fgets(3) for a file descriptor.
 */
char *
rcv_gets(char *buf, size_t len, int fd)
{
        int nr;
        char *p;

        if ((nr = read(fd, buf, len - 1)) == -1)
                return (NULL);
        buf[nr] = '\0';
        if ((p = strchr(buf, '\n')) == NULL)
                return (NULL);
        (void)lseek(fd, (off_t)((p - buf) + 1), SEEK_SET);
        return (buf);
}

/*
 * rcv_mktemp --
 *      Paranoid make temporary file routine.
 */
int
rcv_mktemp(SCR *sp, char *path, char *dname, int perms)
{
        int fd;

        /*
         * !!!
         * We expect mkstemp(3) to set the permissions correctly.  On
         * historic System V systems, mkstemp didn't.  Do it here, on
         * GP's.  This also protects us from users with stupid umasks.
         *
         * XXX
         * The variable perms should really be a mode_t.
         */
        if ((fd = mkstemp(path)) == -1 || fchmod(fd, perms) == -1) {
                msgq_str(sp, M_SYSERR, dname, "%s");
                if (fd != -1) {
                        close(fd);
                        unlink(path);
                        fd = -1;
                }
        }
        return (fd);
}

/*
 * rcv_email --
 *      Send email.
 */
void
rcv_email(SCR *sp, int fd)
{
        struct stat sb;
        pid_t pid;

        /*
         * In secure mode, our pledge(2) includes neither "proc"
         * nor "exec".  So simply skip sending the mail.
         * Later vi -r still works because rcv_mailfile()
         * already did all the necessary setup.
         */
        if (O_ISSET(sp, O_SECURE))
                return;

        if (_PATH_SENDMAIL[0] != '/' || stat(_PATH_SENDMAIL, &sb) == -1)
                msgq_str(sp, M_SYSERR,
                    _PATH_SENDMAIL, "not sending email: %s");
        else {
                /*
                 * !!!
                 * If you need to port this to a system that doesn't have
                 * sendmail, the -t flag causes sendmail to read the message
                 * for the recipients instead of specifying them some other
                 * way.
                 */
                switch (pid = fork()) {
                case -1:                /* Error. */
                        msgq(sp, M_SYSERR, "fork");
                        break;
                case 0:                 /* Sendmail. */
                        if (lseek(fd, 0, SEEK_SET) == -1) {
                                msgq(sp, M_SYSERR, "lseek");
                                _exit(127);
                        }
                        if (fd != STDIN_FILENO) {
                                if (dup2(fd, STDIN_FILENO) == -1) {
                                        msgq(sp, M_SYSERR, "dup2");
                                        _exit(127);
                                }
                                close(fd);
                        }
                        execl(_PATH_SENDMAIL, "sendmail", "-t", (char *)NULL);
                        msgq(sp, M_SYSERR, _PATH_SENDMAIL);
                        _exit(127);
                default:                /* Parent. */
                        while (waitpid(pid, NULL, 0) == -1 && errno == EINTR)
                                continue;
                        break;
                }

        }
}
