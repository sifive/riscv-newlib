/* Copyright (c) 2017  SiFive Inc. All rights reserved.

   This copyrighted material is made available to anyone wishing to use,
   modify, copy, or redistribute it subject to the terms and conditions
   of the FreeBSD License.   This program is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
   including the implied warranties of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  A copy of this license is available at
   http://www.opensource.org/licenses.
*/

#include "newlib.h"

#=========================================================================
# crt0.S : Entry point for RISC-V user programs
#=========================================================================

  .text
  .global _start
  .type   _start, @function
_start:
  # Initialize global pointer
.option push
.option norelax
#ifdef __riscv_cmodel_compact
1:auipc        a0, %pcrel_hi(__global_pointer__)
  addi a0, a0, %pcrel_lo(1b)
  ld   gp, 0(a0)
  add  gp, gp, a0
#else
1:auipc gp, %pcrel_hi(__global_pointer$)
  addi  gp, gp, %pcrel_lo(1b)
#endif
.option pop

  # Clear the bss segment
#ifdef __riscv_cmodel_compact
  lla.gprel    a0, _edata
  lla.gprel    a2, _end
#else
  la      a0, _edata
  la      a2, _end
#endif
  sub     a2, a2, a0
  li      a1, 0
  call    memset
#ifdef _LITE_EXIT
  # Make reference to atexit weak to avoid unconditionally pulling in
  # support code.  Refer to comments in __atexit.c for more details.
  .weak   atexit
#ifdef __riscv_cmodel_compact
  la.got.gprel a0, atexit
#else
  la      a0, atexit
#endif
  beqz    a0, .Lweak_atexit
  .weak   __libc_fini_array
#endif

#ifdef __riscv_cmodel_compact
  la.got.gprel a0, __libc_fini_array
#else
  la      a0, __libc_fini_array   # Register global termination functions
#endif
  call    atexit                  #  to be called upon exit
#ifdef _LITE_EXIT
.Lweak_atexit:
#endif
  call    __libc_init_array       # Run global initialization functions

  lw      a0, 0(sp)                  # a0 = argc
  addi    a1, sp, __SIZEOF_POINTER__ # a1 = argv
  li      a2, 0                      # a2 = envp = NULL
  call    main
  tail    exit
  .size  _start, .-_start

#ifdef __riscv_cmodel_compact
  .section .text.__global_pointer__, "aMG",@progbits, 8, __global_pointer__, comdat
  .align 3
  .global __global_pointer__
  .type   __global_pointer__, object
__global_pointer__:
  .quad   __global_pointer$ -.
#endif
