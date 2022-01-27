#define __NEED_OPENBSD_open

#include "../include/compat.h"

#include <sys/file.h>

#include <bsd_fcntl.h>
#include <stdarg.h>
#include <bsd_unistd.h>

#include <stdio.h>

#undef open

int
openbsd_open(const char *path, int flags, ...) 
{
	va_list ap;
	int fd, lock;

	lock = flags & (O_EXLOCK | O_SHLOCK);
	flags &= ~lock;

	va_start(ap, flags);
	if ((fd = open(path, flags, ap)) == -1) 
		return -1;
	va_end(ap);

	if (lock == 0)
		return fd;

	if (flock(fd, lock & O_EXLOCK ? LOCK_EX : LOCK_SH) == -1) {
		close(fd);
		return -1;
	}

	return fd;
}
