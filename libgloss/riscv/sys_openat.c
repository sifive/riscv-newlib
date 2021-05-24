#include <machine/syscall.h>
#include "internal_syscall.h"

#ifdef USING_QEMU
#include <fcntl.h>

/* And also using differnt value for open's flags.  */
#define LINUX_O_CREAT 0100
#define LINUX_O_TRUNC 01000
#define LINUX_O_APPEND 02000

static int traslate_flags(int flags)
{
  /* O_WRONLY and O_RDWR are same as Linux.  */
  int new_flags = flags & (O_RDWR | O_WRONLY);

  if (flags & O_CREAT)
    new_flags |= LINUX_O_CREAT;

  if (flags & O_TRUNC)
    new_flags |= LINUX_O_TRUNC;

  if (flags & O_APPEND)
    new_flags |= LINUX_O_APPEND;

  return new_flags;
}
#endif

/* Open file relative to given directory.  */
int _openat(int dirfd, const char *name, int flags, int mode)
{
  int traslated_flags_dirfd = dirfd;
  int traslated_flags = flags;

#ifdef USING_QEMU
  traslated_flags = traslate_flags (flags);
  if (dirfd == AT_FDCWD)
    traslated_flags_dirfd = LINUX_AT_FDCWD;
#endif

  return syscall_errno (SYS_openat, 4, traslated_flags_dirfd,
			name, traslated_flags, mode, 0, 0);
}
