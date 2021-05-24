#include <machine/syscall.h>
#include "internal_syscall.h"

/* Remove a file's directory entry.  */
int
_unlink(const char *name)
{
#ifdef USING_QEMU
  return syscall_errno (SYS_unlinkat, 3, LINUX_AT_FDCWD, name, 0, 0, 0, 0);
#else
  return syscall_errno (SYS_unlink, 1, name, 0, 0, 0, 0, 0);
#endif
}
