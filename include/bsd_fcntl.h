#include_next <fcntl.h>

#ifndef _COMPAT_FCNTL_H_
#define _COMPAT_FCNTL_H_

#define open openbsd_open

#ifdef O_EXLOCK
# undef O_EXLOCK
#endif /* ifdef O_EXLOCK */
#define O_EXLOCK 10000000

#ifdef O_SHLOCK
# undef O_SHLOCK
#endif /* ifdef O_SHLOCK */
#define O_SHLOCK 20000000

int openbsd_open(const char *, int, ...);

#endif /* !_COMPAT_FCNTL_H_ */
