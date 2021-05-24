#include <machine/syscall.h>
#include "kernel_stat.h"
#include "internal_syscall.h"

/* Status of a file (by name).  */

/* Newlib and linux using differnt value for AT_FDCWD.  */
#define LINUX_AT_FDCWD           -100

int
_stat(const char *file, struct stat *st)
{
  struct kernel_stat kst;
  int rv;
#ifdef USING_QEMU
  /* RISC-V Linux provide newfstatat/fstatat64, and newfstatat
     only that require 4 argument.  */
  rv = syscall_errno (SYS_fstatat, 4, LINUX_AT_FDCWD, file, &kst, 0, 0, 0);
#else
  rv = syscall_errno (SYS_stat, 2, file, &kst, 0, 0, 0, 0);
#endif
  _conv_stat (st, &kst);
  return rv;
}
