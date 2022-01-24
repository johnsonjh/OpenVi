#ifndef _COMPAT_SYS_TYPES_H_
#define _COMPAT_SYS_TYPES_H_
/*
 * for major()/minor() macros with glibc, needs to be included before
 * <sys/types.h>
 */
#include <sys/sysmacros.h>
#endif /* _COMPAT_SYS_TYPES_H_ */

#include_next <sys/types.h>
