#include_next <fcntl.h>

#ifndef _COMPAT_FCNTL_H_
#define _COMPAT_FCNTL_H_

#define open openbsd_open

#define O_EXLOCK 10000000
#define O_SHLOCK 20000000

int openbsd_open(const char *, int, ...);

#endif /* !_COMPAT_FCNTL_H_ */
