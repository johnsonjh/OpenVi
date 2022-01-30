#include_next <sys/time.h>

#ifndef _COMPAT_SYS_TIME_H_
# define _COMPAT_SYS_TIME_H_
# include <time.h>

# ifndef TIMEVAL_TO_TIMESPEC
#  define TIMEVAL_TO_TIMESPEC(tv, ts) {					\
	(ts)->tv_sec = (tv)->tv_sec;					\
	(ts)->tv_nsec = (tv)->tv_usec * 1000;				\
}
# endif /* ifndef TIMEVAL_TO_TIMESPEC */

# ifndef TIMESPEC_TO_TIMEVAL
#  define TIMESPEC_TO_TIMEVAL(tv, ts) {					\
	(tv)->tv_sec = (ts)->tv_sec;					\
	(tv)->tv_usec = (ts)->tv_nsec / 1000;				\
}
# endif /* ifndef TIMESPEC_TO_TIMEVAL */

# ifdef timespecclear
#  undef timespecclear
# endif /* ifdef timespecclear */
# define timespecclear(tsp)		(tsp)->tv_sec = (tsp)->tv_nsec = 0

# ifdef timespecisset
#  undef timespecisset
# endif /* ifdef timespecisset */
# define timespecisset(tsp)		((tsp)->tv_sec || (tsp)->tv_nsec)

# ifdef timespeccmp
#  undef timespeccmp
# endif /* ifdef timespeccmp */
# define timespeccmp(tsp, usp, cmp)					\
	(((tsp)->tv_sec == (usp)->tv_sec) ?				\
	    ((tsp)->tv_nsec cmp (usp)->tv_nsec) :			\
	    ((tsp)->tv_sec cmp (usp)->tv_sec))

# ifdef timespecadd
#  undef timespecadd
# endif /* ifdef timespecadd */
# define timespecadd(tsp, usp, vsp)					\
	do {								\
		(vsp)->tv_sec = (tsp)->tv_sec + (usp)->tv_sec;		\
		(vsp)->tv_nsec = (tsp)->tv_nsec + (usp)->tv_nsec;	\
		if ((vsp)->tv_nsec >= 1000000000L) {			\
			(vsp)->tv_sec++;				\
			(vsp)->tv_nsec -= 1000000000L;			\
		}							\
	} while (0)

# ifdef timespecsub
#  undef timespecsub
# endif /* ifdef timespecsub */
# define timespecsub(tsp, usp, vsp)					\
	do {								\
		(vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;		\
		(vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec;	\
		if ((vsp)->tv_nsec < 0) {				\
			(vsp)->tv_sec--;				\
			(vsp)->tv_nsec += 1000000000L;			\
		}							\
	} while (0)

#endif /* _COMPAT_SYS_TIME_H_ */
