#ifndef _COMPAT_SYS_TYPES_H_
# define _COMPAT_SYS_TYPES_H_
/*
 * for major()/minor() macros with glibc, needs to be included before
 * <sys/types.h>
 */
# if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ)
#  include <sys/sysmacros.h>
# endif /* if defined(__GNU_LIBRARY__) && defined(__GLIBC_PREREQ) */
#endif /* _COMPAT_SYS_TYPES_H_ */

#include_next <sys/types.h>
