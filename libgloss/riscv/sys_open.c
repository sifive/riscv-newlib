#include <machine/syscall.h>
#include "internal_syscall.h"

#ifdef USING_QEMU
#include <fcntl.h>

extern int _openat(int dirfd, const char *name, int flags, int mode);
#endif

/* Open a file.  */
int
_open(const char *name, int flags, int mode)
{
#ifdef USING_QEMU
  return _openat (AT_FDCWD, name, flags, mode);
#else
  return syscall_errno (SYS_open, 3, name, flags, mode, 0, 0, 0);
#endif
}
