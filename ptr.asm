.section .text
.globl main
main:
  addi sp, sp, -112
main_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 16(sp)
  addi t1, sp, 8
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 0(t1)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 24(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  sd t0, 32(sp)
  addi t1, sp, 0
  li t3, 0
  add t1, t1, t3
  li t3, 1
  add t1, t1, t3
  li t0, 65
  sb t0, 0(t1)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 40(sp)
  ld t0, 40(sp)
  li t3, 2
  add t0, t0, t3
  sd t0, 48(sp)
  ld t1, 40(sp)
  li t3, 2
  add t1, t1, t3
  li t0, 66
  sb t0, 0(t1)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 56(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  sd t0, 64(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 72(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 80(sp)
  ld t0, 80(sp)
  li t3, 2
  add t0, t0, t3
  sd t0, 88(sp)
  ld t0, 80(sp)
  li t3, 2
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 96(sp)
  lb t0, 72(sp)
  lb t1, 96(sp)
  add t2, t0, t1
  sb t2, 97(sp)
  lb a0, 97(sp)
  addi sp, sp, 112
  ret
