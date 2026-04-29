.section .bss
.globl buf
.align 1
buf:
  .zero 1024
.section .rodata
.globl .str.0
.align 1
.str.0:
  .byte 103
  .byte 114
  .byte 101
  .byte 112
  .byte 58
  .byte 32
  .byte 114
  .byte 101
  .byte 97
  .byte 100
  .byte 32
  .byte 101
  .byte 114
  .byte 114
  .byte 111
  .byte 114
  .byte 10
  .byte 0
.section .rodata
.globl .str.1
.align 1
.str.1:
  .byte 37
  .byte 115
  .byte 58
  .byte 0
.section .rodata
.globl .str.2
.align 1
.str.2:
  .byte 37
  .byte 115
  .byte 10
  .byte 0
.section .rodata
.globl .str.3
.align 1
.str.3:
  .byte 117
  .byte 115
  .byte 97
  .byte 103
  .byte 101
  .byte 58
  .byte 32
  .byte 103
  .byte 114
  .byte 101
  .byte 112
  .byte 32
  .byte 112
  .byte 97
  .byte 116
  .byte 116
  .byte 101
  .byte 114
  .byte 110
  .byte 32
  .byte 91
  .byte 102
  .byte 105
  .byte 108
  .byte 101
  .byte 32
  .byte 46
  .byte 46
  .byte 46
  .byte 93
  .byte 10
  .byte 0
.section .rodata
.globl .str.4
.align 1
.str.4:
  .byte 0
.section .rodata
.globl .str.5
.align 1
.str.5:
  .byte 103
  .byte 114
  .byte 101
  .byte 112
  .byte 58
  .byte 32
  .byte 99
  .byte 97
  .byte 110
  .byte 110
  .byte 111
  .byte 116
  .byte 32
  .byte 111
  .byte 112
  .byte 101
  .byte 110
  .byte 32
  .byte 37
  .byte 115
  .byte 10
  .byte 0

.section .text
.globl grep
grep:
  addi sp, sp, -528
  sd ra, 520(sp)
grep_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 16
  mv t0, a2
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 24
  mv t0, a3
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 32
  li t0, 0
  sw t0, 0(t1)
  j grep_while.cond
grep_while.cond:
  li t0, 1
  bnez t0, grep_while.body
  j grep_while.end
grep_while.body:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 56(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 64(sp)
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 72(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  lw t3, 72(sp)
  add t0, t0, t3
  sd t0, 80(sp)
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 88(sp)
  li t0, 1024
  lw t1, 88(sp)
  subw t2, t0, t1
  sw t2, 92(sp)
  lw t0, 92(sp)
  li t1, 1
  subw t2, t0, t1
  sw t2, 96(sp)
  lw t0, 56(sp)
  mv a0, t0
  la t0, buf
  li t3, 0
  add t0, t0, t3
  lw t3, 72(sp)
  add t0, t0, t3
  mv a1, t0
  lw t0, 96(sp)
  mv a2, t0
  call read
  sw a0, 100(sp)
  addi t1, sp, 28
  lw t0, 100(sp)
  sw t0, 0(t1)
  addi t0, sp, 28
  lw t1, 0(t0)
  sw t1, 104(sp)
  lw t0, 104(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 108(sp)
  lw t0, 108(sp)
  bnez t0, grep_if.then
  j grep_if.merge
grep_while.end:
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 112(sp)
  lw t0, 112(sp)
  li t1, 0
  slt t2, t1, t0
  sw t2, 116(sp)
  lw t0, 116(sp)
  bnez t0, grep_if.then.6
  j grep_if.merge.6
grep_if.then:
  la t0, .str.0
  li t3, 0
  add t0, t0, t3
  sd t0, 120(sp)
  li t0, 2
  mv a0, t0
  la t0, .str.0
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call fprintf
  sw a0, 128(sp)
  ld ra, 520(sp)
  addi sp, sp, 528
  ret
grep_if.merge:
  addi t0, sp, 28
  lw t1, 0(t0)
  sw t1, 132(sp)
  lw t0, 132(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 136(sp)
  lw t0, 136(sp)
  bnez t0, grep_if.then.1
  j grep_if.merge.1
grep_if.then.1:
  j grep_while.end
grep_if.merge.1:
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 140(sp)
  addi t0, sp, 28
  lw t1, 0(t0)
  sw t1, 144(sp)
  lw t0, 140(sp)
  lw t1, 144(sp)
  addw t2, t0, t1
  sw t2, 148(sp)
  addi t1, sp, 32
  lw t0, 148(sp)
  sw t0, 0(t1)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 152(sp)
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 160(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  lw t3, 160(sp)
  add t0, t0, t3
  sd t0, 168(sp)
  la t1, buf
  li t3, 0
  add t1, t1, t3
  lw t3, 160(sp)
  add t1, t1, t3
  li t0, 0
  sb t0, 0(t1)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 176(sp)
  addi t1, sp, 40
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 0(t1)
  j grep_while.cond.1
grep_while.cond.1:
  li t0, 1
  bnez t0, grep_while.body.1
  j grep_while.end.1
grep_while.body.1:
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 184(sp)
  ld t0, 184(sp)
  mv a0, t0
  li t0, 10
  mv a1, t0
  call strchr
  sd a0, 192(sp)
  addi t1, sp, 48
  ld t0, 192(sp)
  sd t0, 0(t1)
  addi t0, sp, 48
  ld t1, 0(t0)
  sd t1, 200(sp)
  ld t0, 200(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 208(sp)
  lw t0, 208(sp)
  bnez t0, grep_if.then.2
  j grep_if.merge.2
grep_while.end.1:
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 216(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 224(sp)
  ld t0, 216(sp)
  la t1, buf
  li t3, 0
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 232(sp)
  lw t0, 232(sp)
  bnez t0, grep_if.then.5
  j grep_if.merge.5
grep_if.then.2:
  j grep_while.end.1
grep_if.merge.2:
  addi t0, sp, 48
  ld t1, 0(t0)
  sd t1, 240(sp)
  ld t1, 240(sp)
  li t0, 0
  sb t0, 0(t1)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 248(sp)
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 256(sp)
  ld t0, 248(sp)
  mv a0, t0
  ld t0, 256(sp)
  mv a1, t0
  call match
  sw a0, 264(sp)
  lw t0, 264(sp)
  bnez t0, grep_if.then.3
  j grep_if.merge.3
grep_if.then.3:
  addi t0, sp, 24
  lw t1, 0(t0)
  sw t1, 268(sp)
  lw t0, 268(sp)
  bnez t0, grep_if.then.4
  j grep_if.merge.4
grep_if.merge.3:
  addi t0, sp, 48
  ld t1, 0(t0)
  sd t1, 272(sp)
  ld t0, 272(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 280(sp)
  addi t1, sp, 40
  ld t0, 272(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 0(t1)
  j grep_while.cond.1
grep_if.then.4:
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  sd t0, 288(sp)
  addi t0, sp, 16
  ld t1, 0(t0)
  sd t1, 296(sp)
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  ld t0, 296(sp)
  mv a1, t0
  call printf
  sw a0, 304(sp)
  j grep_if.merge.4
grep_if.merge.4:
  addi t0, sp, 48
  ld t1, 0(t0)
  sd t1, 312(sp)
  ld t1, 312(sp)
  li t0, 10
  sb t0, 0(t1)
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 320(sp)
  addi t0, sp, 48
  ld t1, 0(t0)
  sd t1, 328(sp)
  ld t0, 328(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 336(sp)
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 344(sp)
  ld t0, 328(sp)
  li t3, 1
  add t0, t0, t3
  ld t1, 344(sp)
  subw t2, t0, t1
  sw t2, 352(sp)
  li t0, 1
  mv a0, t0
  ld t0, 320(sp)
  mv a1, t0
  lw t0, 352(sp)
  mv a2, t0
  call write
  sw a0, 356(sp)
  j grep_if.merge.3
grep_if.then.5:
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 360(sp)
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 368(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 376(sp)
  ld t0, 368(sp)
  la t1, buf
  li t3, 0
  add t1, t1, t3
  subw t2, t0, t1
  sw t2, 384(sp)
  lw t0, 360(sp)
  lw t1, 384(sp)
  subw t2, t0, t1
  sw t2, 388(sp)
  addi t1, sp, 32
  lw t0, 388(sp)
  sw t0, 0(t1)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 392(sp)
  addi t0, sp, 40
  ld t1, 0(t0)
  sd t1, 400(sp)
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 408(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  ld t0, 400(sp)
  mv a1, t0
  lw t0, 408(sp)
  mv a2, t0
  call memmove
  sd a0, 416(sp)
  j grep_if.merge.5
grep_if.merge.5:
  j grep_while.cond
grep_if.then.6:
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 424(sp)
  addi t0, sp, 32
  lw t1, 0(t0)
  sw t1, 432(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  lw t3, 432(sp)
  add t0, t0, t3
  sd t0, 440(sp)
  la t1, buf
  li t3, 0
  add t1, t1, t3
  lw t3, 432(sp)
  add t1, t1, t3
  li t0, 0
  sb t0, 0(t1)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 448(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 456(sp)
  ld t0, 448(sp)
  mv a0, t0
  la t0, buf
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call match
  sw a0, 464(sp)
  lw t0, 464(sp)
  bnez t0, grep_if.then.7
  j grep_if.merge.7
grep_if.merge.6:
  ld ra, 520(sp)
  addi sp, sp, 528
  ret
grep_if.then.7:
  addi t0, sp, 24
  lw t1, 0(t0)
  sw t1, 468(sp)
  lw t0, 468(sp)
  bnez t0, grep_if.then.8
  j grep_if.merge.8
grep_if.merge.7:
  j grep_if.merge.6
grep_if.then.8:
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  sd t0, 472(sp)
  addi t0, sp, 16
  ld t1, 0(t0)
  sd t1, 480(sp)
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  ld t0, 480(sp)
  mv a1, t0
  call printf
  sw a0, 488(sp)
  j grep_if.merge.8
grep_if.merge.8:
  la t0, .str.2
  li t3, 0
  add t0, t0, t3
  sd t0, 496(sp)
  la t0, buf
  li t3, 0
  add t0, t0, t3
  sd t0, 504(sp)
  la t0, .str.2
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, buf
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call printf
  sw a0, 512(sp)
  j grep_if.merge.7

.section .text
.globl main
main:
  addi sp, sp, -320
  sd ra, 312(sp)
main_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 32(sp)
  lw t0, 32(sp)
  li t1, 2
  slt t2, t0, t1
  sw t2, 36(sp)
  lw t0, 36(sp)
  bnez t0, main_if.then
  j main_if.merge
main_if.then:
  la t0, .str.3
  li t3, 0
  add t0, t0, t3
  sd t0, 40(sp)
  li t0, 2
  mv a0, t0
  la t0, .str.3
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call fprintf
  sw a0, 48(sp)
  li t0, 1
  mv a0, t0
  call exit
  j main_if.merge
main_if.merge:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 52(sp)
  lw t0, 52(sp)
  li t1, 3
  slt t2, t0, t1
  sw t2, 56(sp)
  lw t0, 56(sp)
  bnez t0, main_if.then.1
  j main_if.merge.1
main_if.then.1:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 64(sp)
  ld t0, 64(sp)
  li t3, 1
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 72(sp)
  ld t0, 64(sp)
  li t3, 1
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  ld t1, 0(t0)
  sd t1, 80(sp)
  la t0, .str.4
  li t3, 0
  add t0, t0, t3
  sd t0, 88(sp)
  ld t0, 80(sp)
  mv a0, t0
  li t0, 0
  mv a1, t0
  la t0, .str.4
  li t3, 0
  add t0, t0, t3
  mv a2, t0
  li t0, 0
  mv a3, t0
  call grep
  li t0, 0
  mv a0, t0
  call exit
  j main_if.merge.1
main_if.merge.1:
  addi t1, sp, 24
  li t0, 0
  sw t0, 0(t1)
  addi t1, sp, 28
  li t0, 0
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 96(sp)
  lw t0, 96(sp)
  li t1, 3
  slt t2, t1, t0
  sw t2, 100(sp)
  lw t0, 100(sp)
  bnez t0, main_if.then.2
  j main_if.merge.2
main_if.then.2:
  addi t1, sp, 28
  li t0, 1
  sw t0, 0(t1)
  j main_if.merge.2
main_if.merge.2:
  addi t1, sp, 20
  li t0, 2
  sw t0, 0(t1)
  j main_while.cond
main_while.cond:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 104(sp)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 108(sp)
  lw t0, 104(sp)
  lw t1, 108(sp)
  slt t2, t0, t1
  sw t2, 112(sp)
  lw t0, 112(sp)
  bnez t0, main_while.body
  j main_while.end
main_while.body:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 120(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 128(sp)
  ld t0, 120(sp)
  lw t3, 128(sp)
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 136(sp)
  ld t0, 120(sp)
  lw t3, 128(sp)
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  ld t1, 0(t0)
  sd t1, 144(sp)
  ld t0, 144(sp)
  mv a0, t0
  li t0, 0
  mv a1, t0
  call open
  sw a0, 152(sp)
  addi t1, sp, 16
  lw t0, 152(sp)
  sw t0, 0(t1)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 156(sp)
  lw t0, 156(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 160(sp)
  lw t0, 160(sp)
  bnez t0, main_if.then.3
  j main_if.merge.3
main_while.end:
  addi t0, sp, 24
  lw t1, 0(t0)
  sw t1, 164(sp)
  lw t0, 164(sp)
  mv a0, t0
  call exit
  li a0, 0
  ld ra, 312(sp)
  addi sp, sp, 320
  ret
main_if.then.3:
  la t0, .str.5
  li t3, 0
  add t0, t0, t3
  sd t0, 168(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 176(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 184(sp)
  ld t0, 176(sp)
  lw t3, 184(sp)
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 192(sp)
  ld t0, 176(sp)
  lw t3, 184(sp)
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  ld t1, 0(t0)
  sd t1, 200(sp)
  li t0, 2
  mv a0, t0
  la t0, .str.5
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  ld t0, 200(sp)
  mv a2, t0
  call fprintf
  sw a0, 208(sp)
  addi t1, sp, 24
  li t0, 1
  sw t0, 0(t1)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 212(sp)
  lw t0, 212(sp)
  li t1, 1
  addw t2, t0, t1
  sw t2, 216(sp)
  addi t1, sp, 20
  lw t0, 216(sp)
  sw t0, 0(t1)
  j main_while.cond
main_if.merge.3:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 224(sp)
  ld t0, 224(sp)
  li t3, 1
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 232(sp)
  ld t0, 224(sp)
  li t3, 1
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  ld t1, 0(t0)
  sd t1, 240(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 248(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 256(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 264(sp)
  ld t0, 256(sp)
  lw t3, 264(sp)
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 272(sp)
  ld t0, 256(sp)
  lw t3, 264(sp)
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  ld t1, 0(t0)
  sd t1, 280(sp)
  addi t0, sp, 28
  lw t1, 0(t0)
  sw t1, 288(sp)
  ld t0, 240(sp)
  mv a0, t0
  lw t0, 248(sp)
  mv a1, t0
  ld t0, 280(sp)
  mv a2, t0
  lw t0, 288(sp)
  mv a3, t0
  call grep
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 292(sp)
  lw t0, 292(sp)
  mv a0, t0
  call close
  sw a0, 296(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 300(sp)
  lw t0, 300(sp)
  li t1, 1
  addw t2, t0, t1
  sw t2, 304(sp)
  addi t1, sp, 20
  lw t0, 304(sp)
  sw t0, 0(t1)
  j main_while.cond

.section .text
.globl match
match:
  addi sp, sp, -144
  sd ra, 128(sp)
match_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sd t0, 0(t1)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 16(sp)
  ld t0, 16(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 24(sp)
  ld t0, 16(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 32(sp)
  lb t0, 32(sp)
  li t1, 94
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 36(sp)
  lw t0, 36(sp)
  bnez t0, match_if.then
  j match_if.merge
match_if.then:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 40(sp)
  ld t0, 40(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 48(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 56(sp)
  ld t0, 40(sp)
  li t3, 1
  add t0, t0, t3
  mv a0, t0
  ld t0, 56(sp)
  mv a1, t0
  call matchhere
  sw a0, 64(sp)
  lw a0, 64(sp)
  ld ra, 128(sp)
  addi sp, sp, 144
  ret
match_if.merge:
  j match_while.cond
match_while.cond:
  li t0, 1
  bnez t0, match_while.body
  j match_while.end
match_while.body:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 72(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 80(sp)
  ld t0, 72(sp)
  mv a0, t0
  ld t0, 80(sp)
  mv a1, t0
  call matchhere
  sw a0, 88(sp)
  lw t0, 88(sp)
  bnez t0, match_if.then.1
  j match_if.merge.1
match_while.end:
  li a0, 0
  ld ra, 128(sp)
  addi sp, sp, 144
  ret
match_if.then.1:
  li a0, 1
  ld ra, 128(sp)
  addi sp, sp, 144
  ret
match_if.merge.1:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 96(sp)
  ld t0, 96(sp)
  lb t1, 0(t0)
  sb t1, 104(sp)
  lb t0, 104(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 108(sp)
  lw t0, 108(sp)
  bnez t0, match_if.then.2
  j match_if.merge.2
match_if.then.2:
  j match_while.end
match_if.merge.2:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 112(sp)
  ld t0, 112(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 120(sp)
  addi t1, sp, 8
  ld t0, 112(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 0(t1)
  j match_while.cond

.section .text
.globl matchhere
matchhere:
  addi sp, sp, -336
  sd ra, 320(sp)
matchhere_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sd t0, 0(t1)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 16(sp)
  ld t0, 16(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 24(sp)
  ld t0, 16(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 32(sp)
  lb t0, 32(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 36(sp)
  lw t0, 36(sp)
  bnez t0, matchhere_if.then
  j matchhere_if.merge
matchhere_if.then:
  li a0, 1
  ld ra, 320(sp)
  addi sp, sp, 336
  ret
matchhere_if.merge:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 40(sp)
  ld t0, 40(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 48(sp)
  ld t0, 40(sp)
  li t3, 1
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 56(sp)
  lb t0, 56(sp)
  li t1, 42
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 60(sp)
  lw t0, 60(sp)
  bnez t0, matchhere_if.then.1
  j matchhere_if.merge.1
matchhere_if.then.1:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 64(sp)
  ld t0, 64(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 72(sp)
  ld t0, 64(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 80(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 88(sp)
  ld t0, 88(sp)
  li t3, 2
  add t0, t0, t3
  sd t0, 96(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 104(sp)
  lb t0, 80(sp)
  mv a0, t0
  ld t0, 88(sp)
  li t3, 2
  add t0, t0, t3
  mv a1, t0
  ld t0, 104(sp)
  mv a2, t0
  call matchstar
  sw a0, 112(sp)
  lw a0, 112(sp)
  ld ra, 320(sp)
  addi sp, sp, 336
  ret
matchhere_if.merge.1:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 120(sp)
  ld t0, 120(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 128(sp)
  ld t0, 120(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 136(sp)
  lb t0, 136(sp)
  li t1, 36
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 140(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 144(sp)
  ld t0, 144(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 152(sp)
  ld t0, 144(sp)
  li t3, 1
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 160(sp)
  lb t0, 160(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 164(sp)
  lw t0, 140(sp)
  lw t1, 164(sp)
  and t2, t0, t1
  addiw t2, t2, 0
  sw t2, 168(sp)
  lw t0, 168(sp)
  bnez t0, matchhere_if.then.2
  j matchhere_if.merge.2
matchhere_if.then.2:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 176(sp)
  ld t0, 176(sp)
  lb t1, 0(t0)
  sb t1, 184(sp)
  lb t0, 184(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 188(sp)
  lw a0, 188(sp)
  ld ra, 320(sp)
  addi sp, sp, 336
  ret
matchhere_if.merge.2:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 192(sp)
  ld t0, 192(sp)
  lb t1, 0(t0)
  sb t1, 200(sp)
  lb t0, 200(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 204(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 208(sp)
  ld t0, 208(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 216(sp)
  ld t0, 208(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 224(sp)
  lb t0, 224(sp)
  li t1, 46
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 228(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 232(sp)
  ld t0, 232(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 240(sp)
  ld t0, 232(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 248(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 256(sp)
  ld t0, 256(sp)
  lb t1, 0(t0)
  sb t1, 264(sp)
  lb t0, 248(sp)
  lb t1, 264(sp)
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 268(sp)
  lw t0, 228(sp)
  lw t1, 268(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 272(sp)
  lw t0, 204(sp)
  lw t1, 272(sp)
  and t2, t0, t1
  addiw t2, t2, 0
  sw t2, 276(sp)
  lw t0, 276(sp)
  bnez t0, matchhere_if.then.3
  j matchhere_if.merge.3
matchhere_if.then.3:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 280(sp)
  ld t0, 280(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 288(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 296(sp)
  ld t0, 296(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 304(sp)
  ld t0, 280(sp)
  li t3, 1
  add t0, t0, t3
  mv a0, t0
  ld t0, 296(sp)
  li t3, 1
  add t0, t0, t3
  mv a1, t0
  call matchhere
  sw a0, 312(sp)
  lw a0, 312(sp)
  ld ra, 320(sp)
  addi sp, sp, 336
  ret
matchhere_if.merge.3:
  li a0, 0
  ld ra, 320(sp)
  addi sp, sp, 336
  ret

.section .text
.globl matchstar
matchstar:
  addi sp, sp, -128
  sd ra, 112(sp)
matchstar_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 16
  mv t0, a2
  sd t0, 0(t1)
  j matchstar_while.cond
matchstar_while.cond:
  li t0, 1
  bnez t0, matchstar_while.body
  j matchstar_while.end
matchstar_while.body:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 24(sp)
  addi t0, sp, 16
  ld t1, 0(t0)
  sd t1, 32(sp)
  ld t0, 24(sp)
  mv a0, t0
  ld t0, 32(sp)
  mv a1, t0
  call matchhere
  sw a0, 40(sp)
  lw t0, 40(sp)
  bnez t0, matchstar_if.then
  j matchstar_if.merge
matchstar_while.end:
  li a0, 0
  ld ra, 112(sp)
  addi sp, sp, 128
  ret
matchstar_if.then:
  li a0, 1
  ld ra, 112(sp)
  addi sp, sp, 128
  ret
matchstar_if.merge:
  addi t0, sp, 16
  ld t1, 0(t0)
  sd t1, 48(sp)
  ld t0, 48(sp)
  lb t1, 0(t0)
  sb t1, 56(sp)
  lb t0, 56(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 60(sp)
  lw t0, 60(sp)
  bnez t0, matchstar_if.then.1
  j matchstar_if.merge.1
matchstar_if.then.1:
  j matchstar_while.end
matchstar_if.merge.1:
  addi t0, sp, 16
  ld t1, 0(t0)
  sd t1, 64(sp)
  ld t0, 64(sp)
  lb t1, 0(t0)
  sb t1, 72(sp)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 76(sp)
  lb t0, 72(sp)
  lw t1, 76(sp)
  xor t2, t0, t1
  snez t2, t2
  sw t2, 80(sp)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 84(sp)
  lw t0, 84(sp)
  li t1, 46
  xor t2, t0, t1
  snez t2, t2
  sw t2, 88(sp)
  lw t0, 80(sp)
  lw t1, 88(sp)
  and t2, t0, t1
  addiw t2, t2, 0
  sw t2, 92(sp)
  lw t0, 92(sp)
  bnez t0, matchstar_if.then.2
  j matchstar_if.merge.2
matchstar_if.then.2:
  j matchstar_while.end
matchstar_if.merge.2:
  addi t0, sp, 16
  ld t1, 0(t0)
  sd t1, 96(sp)
  ld t0, 96(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 104(sp)
  addi t1, sp, 16
  ld t0, 96(sp)
  li t3, 1
  add t0, t0, t3
  sd t0, 0(t1)
  j matchstar_while.cond
