.section .rodata
.globl .str.0
.align 1
.str.0:
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 100
  .byte 32
  .byte 37
  .byte 115
  .byte 10
  .byte 0
.section .rodata
.globl .str.1
.align 1
.str.1:
  .byte 111
  .byte 107
  .byte 0

.section .text
.globl main
main:
  addi sp, sp, -64
  sd ra, 48(sp)
main_entry:
  la t0, .str.0
  li t3, 0
  add t0, t0, t3
  sd t0, 24(sp)
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  sd t0, 32(sp)
  la t0, .str.0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 1
  mv a1, t0
  li t0, 2
  mv a2, t0
  li t0, 3
  mv a3, t0
  li t0, 4
  mv a4, t0
  li t0, 5
  mv a5, t0
  li t0, 6
  mv a6, t0
  li t0, 7
  mv a7, t0
  li t0, 8
  sw t0, 0(sp)
  addi t1, sp, 8
  li t0, 9
  sw t0, 0(t1)
  addi t1, sp, 16
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  sd t0, 0(t1)
  call printf
  sw a0, 40(sp)
  lw a0, 40(sp)
  ld ra, 48(sp)
  addi sp, sp, 64
  ret
