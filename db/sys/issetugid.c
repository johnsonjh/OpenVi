#include <errno.h>
#include <sys/auxv.h>

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
