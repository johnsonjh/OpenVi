/* OpenBSD mkstemp function */

#include <errno.h>
#include <bsd_fcntl.h>
#include <stdio.h>
#include <time.h>
#include <bsd_unistd.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <sys/stat.h>
#include <sys/types.h>

int
obsd_mkstemp (char *template_name)
{
  int i, j, fd, len, index;

  srand(time(NULL) ^ getpid());

  static const char letters[]
      = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  if (template_name == NULL || (len = strlen (template_name)) < 6
      || memcmp (template_name + (len - 6), "XXXXXX", 6))
  {
    errno = EINVAL;
    return -1;
  }

  for (index = len - 6; index > 0 && template_name[index - 1] == 'X'; index--)
    ;

  for (i = 0; i >= 0; i++)
  {
    for (j = index; j < len; j++)
    {
      template_name[j] = letters[rand () % 62];
    }
    fd = open(template_name,
              O_RDWR | O_CREAT | O_EXCL | F_WRLCK | S_IREAD | S_IWRITE);
    if (fd != -1)
      return fd;
    if (fd == -1 && errno != EEXIST)
      return -1;
  }

  return -1;
}
