#include <errno.h>
#include <sys/auxv.h>

#ifdef __FreeBSD__
# include <unistd.h>
#else
int
issetugid(void)
{
  int rv = 0;

  errno = 0;
  rv = getauxval(AT_SECURE) != 0;
  if (errno)
    {
      errno = 0;
      rv = 1;
    }

  return rv;
}
#endif /* ifdef __FreeBSD__ */
