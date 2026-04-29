.section .data
.globl g1
.align 4
g1:
  .word 1
.section .data
.globl g2
.align 4
g2:
  .word 2
.section .data
.globl buf
.align 1
buf:
  .byte 97
  .byte 98
  .byte 99
  .byte 0
.section .bss
.globl zeros
.align 1
zeros:
  .zero 8

.section .text
.globl main
main:
  addi sp, sp, -64
main_entry:
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 0(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  sd t0, 8(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 16(sp)
  la t0, g1
  lw t1, 0(t0)
  sw t1, 20(sp)
  lb t0, 16(sp)
  lw t1, 20(sp)
  add t2, t0, t1
  sb t2, 24(sp)
  la t0, g2
  lw t1, 0(t0)
  sw t1, 28(sp)
  lb t0, 24(sp)
  lw t1, 28(sp)
  add t2, t0, t1
  sb t2, 32(sp)
  la t0, zeros
  li t3, 0
  add t0, t0, t3
  sd t0, 40(sp)
  la t0, zeros
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  sd t0, 48(sp)
  la t0, zeros
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 56(sp)
  lb t0, 32(sp)
  lb t1, 56(sp)
  add t2, t0, t1
  sb t2, 57(sp)
  lb a0, 57(sp)
  addi sp, sp, 64
  ret
