.section .init, "ax"
.global _start
.p2align 2
.type _start,@function
_start: 
  .cfi_startproc
  .cfi_undefined ra
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop
  la sp, _estack
  add s0, sp, zero
  jal zero, upy_start
  .cfi_endproc
1:
    wfi
    j 1b
  .end
