#include_next <fcntl.h>

#ifndef _COMPAT_FCNTL_H_
#define _COMPAT_FCNTL_H_

#ifdef	__NEED_OPENBSD_open
#define open openbsd_open

#define O_EXLOCK 10000000
#define O_SHLOCK 20000000
#endif /* !__NEED_OPENBSD_open */

int openbsd_open(const char *, int, ...);

#endif /* !_COMPAT_FCNTL_H_ */
