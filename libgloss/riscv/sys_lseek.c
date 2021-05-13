#include <machine/syscall.h>
#include <sys/types.h>
#include "internal_syscall.h"

/* Set position in a file.  */
off_t
_lseek(int file, off_t ptr, int dir)
{
#if __riscv_xlen == 32
  /* Emulate lseek by llseek, because RV32 linux only provide llseek.  */
  long long res;
  int rv;
  rv = syscall_errno (SYS_llseek, 5, file, 0, ptr, (long)&res, dir, 0);
  if (rv >= 0)
    return res;
  else
    return rv;
#else
  return syscall_errno (SYS_lseek, 3, file, ptr, dir, 0, 0, 0);
#endif
}
