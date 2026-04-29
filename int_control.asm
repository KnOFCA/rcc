.section .text
.globl main
main:
  addi sp, sp, -48
main_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  li t0, 1
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 4
  li t0, 2
  sw t0, 0(t1)
  j main_while.cond
main_while.cond:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 8(sp)
  lw t0, 8(sp)
  li t1, 7
  slt t2, t0, t1
  sw t2, 12(sp)
  lw t0, 12(sp)
  bnez t0, main_while.body
  j main_while.end
main_while.body:
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 16(sp)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 20(sp)
  lw t0, 16(sp)
  lw t1, 20(sp)
  addw t2, t0, t1
  sw t2, 24(sp)
  addi t1, sp, 4
  lw t0, 24(sp)
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 28(sp)
  lw t0, 28(sp)
  li t1, 1
  addw t2, t0, t1
  sw t2, 32(sp)
  addi t1, sp, 0
  lw t0, 32(sp)
  sw t0, 0(t1)
  j main_while.cond
main_while.end:
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 36(sp)
  lw t0, 36(sp)
  li t1, 20
  slt t2, t1, t0
  sw t2, 40(sp)
  lw t0, 40(sp)
  bnez t0, main_if.then
  j main_if.merge
main_if.then:
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 44(sp)
  lw a0, 44(sp)
  addi sp, sp, 48
  ret
main_if.merge:
  li a0, 0
  addi sp, sp, 48
  ret
