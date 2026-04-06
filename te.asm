.section .data
.globl O_RDONLY
.align 4
O_RDONLY:
  .word 0
.section .data
.globl O_RDWR
.align 4
O_RDWR:
  .word 2
.section .data
.globl O_CREATE
.align 4
O_CREATE:
  .word 512
.section .data
.globl O_TRUNC
.align 4
O_TRUNC:
  .word 1024
.section .data
.globl ok_msg
.align 1
ok_msg:
  .byte 82
  .byte 67
  .byte 67
  .byte 95
  .byte 85
  .byte 83
  .byte 69
  .byte 82
  .byte 95
  .byte 65
  .byte 76
  .byte 76
  .byte 32
  .byte 79
  .byte 75
  .byte 10
  .byte 0
.section .data
.globl fail_msg
.align 1
fail_msg:
  .byte 82
  .byte 67
  .byte 67
  .byte 95
  .byte 85
  .byte 83
  .byte 69
  .byte 82
  .byte 95
  .byte 65
  .byte 76
  .byte 76
  .byte 32
  .byte 70
  .byte 65
  .byte 73
  .byte 76
  .byte 10
  .byte 0
.section .data
.globl dir_name
.align 1
dir_name:
  .byte 114
  .byte 99
  .byte 99
  .byte 95
  .byte 117
  .byte 115
  .byte 101
  .byte 114
  .byte 95
  .byte 100
  .byte 105
  .byte 114
  .byte 0
.section .data
.globl file_name
.align 1
file_name:
  .byte 98
  .byte 97
  .byte 115
  .byte 101
  .byte 46
  .byte 116
  .byte 120
  .byte 116
  .byte 0
.section .data
.globl link_name
.align 1
link_name:
  .byte 104
  .byte 97
  .byte 114
  .byte 100
  .byte 46
  .byte 116
  .byte 120
  .byte 116
  .byte 0
.section .data
.globl node_name
.align 1
node_name:
  .byte 114
  .byte 99
  .byte 99
  .byte 95
  .byte 110
  .byte 111
  .byte 100
  .byte 101
  .byte 0
.section .data
.globl echo_prog
.align 1
echo_prog:
  .byte 101
  .byte 99
  .byte 104
  .byte 111
  .byte 0
.section .data
.globl exec_word
.align 1
exec_word:
  .byte 69
  .byte 88
  .byte 69
  .byte 67
  .byte 0
.section .rodata
.globl .str.0
.align 1
.str.0:
  .byte 46
  .byte 46
  .byte 0
.section .rodata
.globl .str.1
.align 1
.str.1:
  .byte 97
  .byte 108
  .byte 112
  .byte 104
  .byte 97
  .byte 52
  .byte 50
  .byte 0
.section .rodata
.globl .str.2
.align 1
.str.2:
  .byte 97
  .byte 108
  .byte 112
  .byte 104
  .byte 97
  .byte 52
  .byte 51
  .byte 0
.section .rodata
.globl .str.3
.align 1
.str.3:
  .byte 48
  .byte 49
  .byte 50
  .byte 51
  .byte 52
  .byte 53
  .byte 54
  .byte 55
  .byte 56
  .byte 57
  .byte 0
.section .rodata
.globl .str.4
.align 1
.str.4:
  .byte 50
  .byte 48
  .byte 52
  .byte 56
  .byte 0
.section .rodata
.globl .str.5
.align 1
.str.5:
  .byte 80
  .byte 37
  .byte 100
  .byte 37
  .byte 115
  .byte 10
  .byte 0
.section .rodata
.globl .str.6
.align 1
.str.6:
  .byte 81
  .byte 0
.section .rodata
.globl .str.7
.align 1
.str.7:
  .byte 69
  .byte 37
  .byte 100
  .byte 37
  .byte 115
  .byte 10
  .byte 0
.section .rodata
.globl .str.8
.align 1
.str.8:
  .byte 82
  .byte 0
.section .rodata
.globl .str.9
.align 1
.str.9:
  .byte 108
  .byte 105
  .byte 110
  .byte 101
  .byte 10
  .byte 0

.section .text
.globl fail
fail:
  addi sp, sp, -32
  sd ra, 16(sp)
fail_entry:
  la t0, fail_msg
  li t3, 0
  add t0, t0, t3
  sd t0, 0(sp)
  li t0, 1
  mv a0, t0
  la t0, fail_msg
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 18
  mv a2, t0
  call write
  sw a0, 8(sp)
  li t0, 1
  mv a0, t0
  call exit
  ld ra, 16(sp)
  addi sp, sp, 32
  ret

.section .text
.globl same_bytes
same_bytes:
  addi sp, sp, -112
same_bytes_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 16
  mv t0, a2
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 20
  li t0, 0
  sw t0, 0(t1)
  j same_bytes_while.cond
same_bytes_while.cond:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 24(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 28(sp)
  lw t0, 24(sp)
  lw t1, 28(sp)
  slt t2, t0, t1
  sw t2, 32(sp)
  lw t0, 32(sp)
  bnez t0, same_bytes_while.body
  j same_bytes_while.end
same_bytes_while.body:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 40(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 48(sp)
  ld t0, 40(sp)
  lw t3, 48(sp)
  add t0, t0, t3
  sd t0, 56(sp)
  ld t0, 40(sp)
  lw t3, 48(sp)
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 64(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 72(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 80(sp)
  ld t0, 72(sp)
  lw t3, 80(sp)
  add t0, t0, t3
  sd t0, 88(sp)
  ld t0, 72(sp)
  lw t3, 80(sp)
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 96(sp)
  lb t0, 64(sp)
  lb t1, 96(sp)
  xor t2, t0, t1
  snez t2, t2
  sw t2, 100(sp)
  lw t0, 100(sp)
  bnez t0, same_bytes_if.then
  j same_bytes_if.merge
same_bytes_while.end:
  li a0, 1
  addi sp, sp, 112
  ret
same_bytes_if.then:
  li a0, 0
  addi sp, sp, 112
  ret
same_bytes_if.merge:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 104(sp)
  lw t0, 104(sp)
  li t1, 1
  addw t2, t0, t1
  sw t2, 108(sp)
  addi t1, sp, 20
  lw t0, 108(sp)
  sw t0, 0(t1)
  j same_bytes_while.cond

.section .text
.globl read_full
read_full:
  addi sp, sp, -128
  sd ra, 120(sp)
read_full_entry:
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
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 20
  li t0, 0
  sw t0, 0(t1)
  j read_full_while.cond
read_full_while.cond:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 28(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 32(sp)
  lw t0, 28(sp)
  lw t1, 32(sp)
  slt t2, t0, t1
  sw t2, 36(sp)
  lw t0, 36(sp)
  bnez t0, read_full_while.body
  j read_full_while.end
read_full_while.body:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 40(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 48(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 56(sp)
  ld t0, 48(sp)
  lw t3, 56(sp)
  add t0, t0, t3
  sd t0, 64(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 72(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 76(sp)
  lw t0, 72(sp)
  lw t1, 76(sp)
  subw t2, t0, t1
  sw t2, 80(sp)
  lw t0, 40(sp)
  mv a0, t0
  ld t0, 48(sp)
  lw t3, 56(sp)
  add t0, t0, t3
  mv a1, t0
  lw t0, 80(sp)
  mv a2, t0
  call read
  sw a0, 84(sp)
  addi t1, sp, 24
  lw t0, 84(sp)
  sw t0, 0(t1)
  addi t0, sp, 24
  lw t1, 0(t0)
  sw t1, 88(sp)
  lw t0, 88(sp)
  li t1, 0
  slt t2, t1, t0
  xori t2, t2, 1
  sw t2, 92(sp)
  lw t0, 92(sp)
  bnez t0, read_full_if.then
  j read_full_if.merge
read_full_while.end:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 96(sp)
  lw a0, 96(sp)
  ld ra, 120(sp)
  addi sp, sp, 128
  ret
read_full_if.then:
  li t0, 0
  li t1, 1
  subw t2, t0, t1
  sw t2, 100(sp)
  lw a0, 100(sp)
  ld ra, 120(sp)
  addi sp, sp, 128
  ret
read_full_if.merge:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 104(sp)
  addi t0, sp, 24
  lw t1, 0(t0)
  sw t1, 108(sp)
  lw t0, 104(sp)
  lw t1, 108(sp)
  addw t2, t0, t1
  sw t2, 112(sp)
  addi t1, sp, 20
  lw t0, 112(sp)
  sw t0, 0(t1)
  j read_full_while.cond

.section .text
.globl fill_bytes
fill_bytes:
  addi sp, sp, -224
fill_bytes_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 12
  mv t0, a2
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 16
  li t0, 0
  sw t0, 0(t1)
  j fill_bytes_while.cond
fill_bytes_while.cond:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 20(sp)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 24(sp)
  lw t0, 20(sp)
  lw t1, 24(sp)
  slt t2, t0, t1
  sw t2, 28(sp)
  lw t0, 28(sp)
  bnez t0, fill_bytes_while.body
  j fill_bytes_while.end
fill_bytes_while.body:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 32(sp)
  lw t0, 32(sp)
  li t1, 3
  remw t2, t0, t1
  sw t2, 36(sp)
  lw t0, 36(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 40(sp)
  lw t0, 40(sp)
  bnez t0, fill_bytes_if.then
  j fill_bytes_if.else
fill_bytes_while.end:
  addi sp, sp, 224
  ret
fill_bytes_if.then:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 48(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 56(sp)
  ld t0, 48(sp)
  lw t3, 56(sp)
  add t0, t0, t3
  sd t0, 64(sp)
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 72(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 76(sp)
  lw t0, 72(sp)
  lw t1, 76(sp)
  addw t2, t0, t1
  sw t2, 80(sp)
  lw t0, 80(sp)
  li t1, 26
  remw t2, t0, t1
  sw t2, 84(sp)
  li t0, 97
  lw t1, 84(sp)
  addw t2, t0, t1
  sw t2, 88(sp)
  lw t0, 88(sp)
  sb t0, 92(sp)
  ld t1, 48(sp)
  lw t3, 56(sp)
  add t1, t1, t3
  lb t0, 92(sp)
  sb t0, 0(t1)
  j fill_bytes_if.merge
fill_bytes_if.else:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 96(sp)
  lw t0, 96(sp)
  li t1, 3
  remw t2, t0, t1
  sw t2, 100(sp)
  lw t0, 100(sp)
  li t1, 1
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 104(sp)
  lw t0, 104(sp)
  bnez t0, fill_bytes_if.then.1
  j fill_bytes_if.else.1
fill_bytes_if.merge:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 108(sp)
  lw t0, 108(sp)
  li t1, 1
  addw t2, t0, t1
  sw t2, 112(sp)
  addi t1, sp, 16
  lw t0, 112(sp)
  sw t0, 0(t1)
  j fill_bytes_while.cond
fill_bytes_if.then.1:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 120(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 128(sp)
  ld t0, 120(sp)
  lw t3, 128(sp)
  add t0, t0, t3
  sd t0, 136(sp)
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 144(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 148(sp)
  lw t0, 144(sp)
  lw t1, 148(sp)
  addw t2, t0, t1
  sw t2, 152(sp)
  lw t0, 152(sp)
  li t1, 26
  remw t2, t0, t1
  sw t2, 156(sp)
  li t0, 65
  lw t1, 156(sp)
  addw t2, t0, t1
  sw t2, 160(sp)
  lw t0, 160(sp)
  sb t0, 164(sp)
  ld t1, 120(sp)
  lw t3, 128(sp)
  add t1, t1, t3
  lb t0, 164(sp)
  sb t0, 0(t1)
  j fill_bytes_if.merge.1
fill_bytes_if.else.1:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 168(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 176(sp)
  ld t0, 168(sp)
  lw t3, 176(sp)
  add t0, t0, t3
  sd t0, 184(sp)
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 192(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 196(sp)
  lw t0, 192(sp)
  lw t1, 196(sp)
  addw t2, t0, t1
  sw t2, 200(sp)
  lw t0, 200(sp)
  li t1, 10
  remw t2, t0, t1
  sw t2, 204(sp)
  li t0, 48
  lw t1, 204(sp)
  addw t2, t0, t1
  sw t2, 208(sp)
  lw t0, 208(sp)
  sb t0, 212(sp)
  ld t1, 168(sp)
  lw t3, 176(sp)
  add t1, t1, t3
  lb t0, 212(sp)
  sb t0, 0(t1)
  j fill_bytes_if.merge.1
fill_bytes_if.merge.1:
  j fill_bytes_if.merge

.section .text
.globl sum_bytes
sum_bytes:
  addi sp, sp, -96
sum_bytes_entry:
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 0
  mv t0, a0
  sd t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 8
  mv t0, a1
  sw t0, 0(t1)
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t1, sp, 12
  li t0, 0
  sw t0, 0(t1)
  addi t1, sp, 16
  li t0, 0
  sw t0, 0(t1)
  j sum_bytes_while.cond
sum_bytes_while.cond:
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 20(sp)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 24(sp)
  lw t0, 20(sp)
  lw t1, 24(sp)
  slt t2, t0, t1
  sw t2, 28(sp)
  lw t0, 28(sp)
  bnez t0, sum_bytes_while.body
  j sum_bytes_while.end
sum_bytes_while.body:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 32(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 40(sp)
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 48(sp)
  ld t0, 40(sp)
  lw t3, 48(sp)
  add t0, t0, t3
  sd t0, 56(sp)
  ld t0, 40(sp)
  lw t3, 48(sp)
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 64(sp)
  lw t0, 32(sp)
  lb t1, 64(sp)
  addw t2, t0, t1
  sw t2, 68(sp)
  addi t1, sp, 16
  lw t0, 68(sp)
  sw t0, 0(t1)
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 72(sp)
  lw t0, 72(sp)
  li t1, 1
  addw t2, t0, t1
  sw t2, 76(sp)
  addi t1, sp, 12
  lw t0, 76(sp)
  sw t0, 0(t1)
  j sum_bytes_while.cond
sum_bytes_while.end:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 80(sp)
  lw a0, 80(sp)
  addi sp, sp, 96
  ret

.section .text
.globl test_basic_proc
test_basic_proc:
  addi sp, sp, -80
  sd ra, 72(sp)
test_basic_proc_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  call getpid
  sw a0, 12(sp)
  addi t1, sp, 0
  lw t0, 12(sp)
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 16(sp)
  lw t0, 16(sp)
  li t1, 0
  slt t2, t1, t0
  xori t2, t2, 1
  sw t2, 20(sp)
  lw t0, 20(sp)
  bnez t0, test_basic_proc_if.then
  j test_basic_proc_if.merge
test_basic_proc_if.then:
  call fail
  j test_basic_proc_if.merge
test_basic_proc_if.merge:
  call fork
  sw a0, 24(sp)
  addi t1, sp, 4
  lw t0, 24(sp)
  sw t0, 0(t1)
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 28(sp)
  lw t0, 28(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 32(sp)
  lw t0, 32(sp)
  bnez t0, test_basic_proc_if.then.1
  j test_basic_proc_if.merge.1
test_basic_proc_if.then.1:
  call fail
  j test_basic_proc_if.merge.1
test_basic_proc_if.merge.1:
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 36(sp)
  lw t0, 36(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 40(sp)
  lw t0, 40(sp)
  bnez t0, test_basic_proc_if.then.2
  j test_basic_proc_if.merge.2
test_basic_proc_if.then.2:
  li t0, 23
  mv a0, t0
  call exit
  j test_basic_proc_if.merge.2
test_basic_proc_if.merge.2:
  li t0, 0
  li t1, 1
  subw t2, t0, t1
  sw t2, 44(sp)
  addi t1, sp, 8
  lw t0, 44(sp)
  sw t0, 0(t1)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 48(sp)
  addi t0, sp, 8
  mv a0, t0
  call wait
  sw a0, 52(sp)
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 56(sp)
  lw t0, 52(sp)
  lw t1, 56(sp)
  xor t2, t0, t1
  snez t2, t2
  sw t2, 60(sp)
  lw t0, 60(sp)
  bnez t0, test_basic_proc_if.then.3
  j test_basic_proc_if.merge.3
test_basic_proc_if.then.3:
  call fail
  j test_basic_proc_if.merge.3
test_basic_proc_if.merge.3:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 64(sp)
  lw t0, 64(sp)
  li t1, 23
  xor t2, t0, t1
  snez t2, t2
  sw t2, 68(sp)
  lw t0, 68(sp)
  bnez t0, test_basic_proc_if.then.4
  j test_basic_proc_if.merge.4
test_basic_proc_if.then.4:
  call fail
  j test_basic_proc_if.merge.4
test_basic_proc_if.merge.4:
  ld ra, 72(sp)
  addi sp, sp, 80
  ret

.section .text
.globl test_kill
test_kill:
  addi sp, sp, -80
  sd ra, 64(sp)
test_kill_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  call fork
  sw a0, 8(sp)
  addi t1, sp, 0
  lw t0, 8(sp)
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 12(sp)
  lw t0, 12(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 16(sp)
  lw t0, 16(sp)
  bnez t0, test_kill_if.then
  j test_kill_if.merge
test_kill_if.then:
  call fail
  j test_kill_if.merge
test_kill_if.merge:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 20(sp)
  lw t0, 20(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 24(sp)
  lw t0, 24(sp)
  bnez t0, test_kill_if.then.1
  j test_kill_if.merge.1
test_kill_if.then.1:
  j test_kill_while.cond
test_kill_if.merge.1:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 28(sp)
  lw t0, 28(sp)
  mv a0, t0
  call kill
  sw a0, 32(sp)
  lw t0, 32(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 36(sp)
  lw t0, 36(sp)
  bnez t0, test_kill_if.then.2
  j test_kill_if.merge.2
test_kill_while.cond:
  li t0, 1
  bnez t0, test_kill_while.body
  j test_kill_while.end
test_kill_while.body:
  li t0, 100
  mv a0, t0
  call sleep
  sw a0, 40(sp)
  j test_kill_while.cond
test_kill_while.end:
  j test_kill_if.merge.1
test_kill_if.then.2:
  call fail
  j test_kill_if.merge.2
test_kill_if.merge.2:
  addi t1, sp, 4
  li t0, 0
  sw t0, 0(t1)
  addi t0, sp, 4
  lw t1, 0(t0)
  sw t1, 44(sp)
  addi t0, sp, 4
  mv a0, t0
  call wait
  sw a0, 48(sp)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 52(sp)
  lw t0, 48(sp)
  lw t1, 52(sp)
  xor t2, t0, t1
  snez t2, t2
  sw t2, 56(sp)
  lw t0, 56(sp)
  bnez t0, test_kill_if.then.3
  j test_kill_if.merge.3
test_kill_if.then.3:
  call fail
  j test_kill_if.merge.3
test_kill_if.merge.3:
  ld ra, 64(sp)
  addi sp, sp, 80
  ret

.section .text
.globl test_pipe_read_write_close_dup
test_pipe_read_write_close_dup:
  addi sp, sp, -352
  sd ra, 344(sp)
test_pipe_read_write_close_dup_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 56(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 19
  mv a1, t0
  li t0, 4
  mv a2, t0
  call fill_bytes
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 64(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  mv a0, t0
  call pipe
  sw a0, 72(sp)
  lw t0, 72(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 76(sp)
  lw t0, 76(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then
  j test_pipe_read_write_close_dup_if.merge
test_pipe_read_write_close_dup_if.then:
  call fail
  j test_pipe_read_write_close_dup_if.merge
test_pipe_read_write_close_dup_if.merge:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 80(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 88(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 96(sp)
  lw t0, 96(sp)
  mv a0, t0
  call dup
  sw a0, 100(sp)
  addi t1, sp, 8
  lw t0, 100(sp)
  sw t0, 0(t1)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 104(sp)
  lw t0, 104(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 108(sp)
  lw t0, 108(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.1
  j test_pipe_read_write_close_dup_if.merge.1
test_pipe_read_write_close_dup_if.then.1:
  call fail
  j test_pipe_read_write_close_dup_if.merge.1
test_pipe_read_write_close_dup_if.merge.1:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 112(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 120(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 128(sp)
  lw t0, 128(sp)
  mv a0, t0
  call close
  sw a0, 132(sp)
  lw t0, 132(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 136(sp)
  lw t0, 136(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.2
  j test_pipe_read_write_close_dup_if.merge.2
test_pipe_read_write_close_dup_if.then.2:
  call fail
  j test_pipe_read_write_close_dup_if.merge.2
test_pipe_read_write_close_dup_if.merge.2:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 144(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 152(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 160(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 168(sp)
  lw t0, 160(sp)
  mv a0, t0
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 6
  mv a2, t0
  call write
  sw a0, 176(sp)
  lw t0, 176(sp)
  li t1, 6
  xor t2, t0, t1
  snez t2, t2
  sw t2, 180(sp)
  lw t0, 180(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.3
  j test_pipe_read_write_close_dup_if.merge.3
test_pipe_read_write_close_dup_if.then.3:
  call fail
  j test_pipe_read_write_close_dup_if.merge.3
test_pipe_read_write_close_dup_if.merge.3:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 184(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 192(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 200(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 208(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  li t3, 6
  add t0, t0, t3
  sd t0, 216(sp)
  lw t0, 200(sp)
  mv a0, t0
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  li t3, 6
  add t0, t0, t3
  mv a1, t0
  li t0, 13
  mv a2, t0
  call write
  sw a0, 224(sp)
  lw t0, 224(sp)
  li t1, 13
  xor t2, t0, t1
  snez t2, t2
  sw t2, 228(sp)
  lw t0, 228(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.4
  j test_pipe_read_write_close_dup_if.merge.4
test_pipe_read_write_close_dup_if.then.4:
  call fail
  j test_pipe_read_write_close_dup_if.merge.4
test_pipe_read_write_close_dup_if.merge.4:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 232(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 240(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 248(sp)
  lw t0, 248(sp)
  mv a0, t0
  call close
  sw a0, 252(sp)
  lw t0, 252(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 256(sp)
  lw t0, 256(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.5
  j test_pipe_read_write_close_dup_if.merge.5
test_pipe_read_write_close_dup_if.then.5:
  call fail
  j test_pipe_read_write_close_dup_if.merge.5
test_pipe_read_write_close_dup_if.merge.5:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 260(sp)
  addi t0, sp, 31
  li t3, 0
  add t0, t0, t3
  sd t0, 264(sp)
  lw t0, 260(sp)
  mv a0, t0
  addi t0, sp, 31
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 19
  mv a2, t0
  call read_full
  sw a0, 272(sp)
  lw t0, 272(sp)
  li t1, 19
  xor t2, t0, t1
  snez t2, t2
  sw t2, 276(sp)
  lw t0, 276(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.6
  j test_pipe_read_write_close_dup_if.merge.6
test_pipe_read_write_close_dup_if.then.6:
  call fail
  j test_pipe_read_write_close_dup_if.merge.6
test_pipe_read_write_close_dup_if.merge.6:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 280(sp)
  addi t0, sp, 31
  li t3, 0
  add t0, t0, t3
  sd t0, 288(sp)
  lw t0, 280(sp)
  mv a0, t0
  addi t0, sp, 31
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 1
  mv a2, t0
  call read
  sw a0, 296(sp)
  lw t0, 296(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 300(sp)
  lw t0, 300(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.7
  j test_pipe_read_write_close_dup_if.merge.7
test_pipe_read_write_close_dup_if.then.7:
  call fail
  j test_pipe_read_write_close_dup_if.merge.7
test_pipe_read_write_close_dup_if.merge.7:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 304(sp)
  lw t0, 304(sp)
  mv a0, t0
  call close
  sw a0, 308(sp)
  lw t0, 308(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 312(sp)
  lw t0, 312(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.8
  j test_pipe_read_write_close_dup_if.merge.8
test_pipe_read_write_close_dup_if.then.8:
  call fail
  j test_pipe_read_write_close_dup_if.merge.8
test_pipe_read_write_close_dup_if.merge.8:
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 320(sp)
  addi t0, sp, 31
  li t3, 0
  add t0, t0, t3
  sd t0, 328(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  addi t0, sp, 31
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 19
  mv a2, t0
  call same_bytes
  sw a0, 336(sp)
  lw t0, 336(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 340(sp)
  lw t0, 340(sp)
  bnez t0, test_pipe_read_write_close_dup_if.then.9
  j test_pipe_read_write_close_dup_if.merge.9
test_pipe_read_write_close_dup_if.then.9:
  call fail
  j test_pipe_read_write_close_dup_if.merge.9
test_pipe_read_write_close_dup_if.merge.9:
  ld ra, 344(sp)
  addi sp, sp, 352
  ret

.section .text
.globl test_sbrk_sleep_uptime
test_sbrk_sleep_uptime:
  addi sp, sp, -192
  sd ra, 184(sp)
test_sbrk_sleep_uptime_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  call uptime
  sw a0, 24(sp)
  addi t1, sp, 16
  lw t0, 24(sp)
  sw t0, 0(t1)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 28(sp)
  lw t0, 28(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 32(sp)
  lw t0, 32(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then
  j test_sbrk_sleep_uptime_if.merge
test_sbrk_sleep_uptime_if.then:
  call fail
  j test_sbrk_sleep_uptime_if.merge
test_sbrk_sleep_uptime_if.merge:
  li t0, 5
  mv a0, t0
  call sleep
  sw a0, 36(sp)
  lw t0, 36(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 40(sp)
  lw t0, 40(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then.1
  j test_sbrk_sleep_uptime_if.merge.1
test_sbrk_sleep_uptime_if.then.1:
  call fail
  j test_sbrk_sleep_uptime_if.merge.1
test_sbrk_sleep_uptime_if.merge.1:
  call uptime
  sw a0, 44(sp)
  addi t1, sp, 20
  lw t0, 44(sp)
  sw t0, 0(t1)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 48(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 52(sp)
  lw t0, 48(sp)
  lw t1, 52(sp)
  slt t2, t0, t1
  sw t2, 56(sp)
  lw t0, 56(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then.2
  j test_sbrk_sleep_uptime_if.merge.2
test_sbrk_sleep_uptime_if.then.2:
  call fail
  j test_sbrk_sleep_uptime_if.merge.2
test_sbrk_sleep_uptime_if.merge.2:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 60(sp)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 64(sp)
  lw t0, 60(sp)
  lw t1, 64(sp)
  subw t2, t0, t1
  sw t2, 68(sp)
  lw t0, 68(sp)
  li t1, 5
  slt t2, t0, t1
  sw t2, 72(sp)
  lw t0, 72(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then.3
  j test_sbrk_sleep_uptime_if.merge.3
test_sbrk_sleep_uptime_if.then.3:
  call fail
  j test_sbrk_sleep_uptime_if.merge.3
test_sbrk_sleep_uptime_if.merge.3:
  li t0, 64
  mv a0, t0
  call sbrk
  sd a0, 80(sp)
  addi t1, sp, 0
  ld t0, 80(sp)
  sd t0, 0(t1)
  li t0, 0
  mv a0, t0
  call sbrk
  sd a0, 88(sp)
  addi t1, sp, 8
  ld t0, 88(sp)
  sd t0, 0(t1)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 96(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 104(sp)
  ld t0, 104(sp)
  li t3, 64
  add t0, t0, t3
  sd t0, 112(sp)
  ld t0, 96(sp)
  ld t1, 104(sp)
  li t3, 64
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 120(sp)
  lw t0, 120(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then.4
  j test_sbrk_sleep_uptime_if.merge.4
test_sbrk_sleep_uptime_if.then.4:
  call fail
  j test_sbrk_sleep_uptime_if.merge.4
test_sbrk_sleep_uptime_if.merge.4:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 128(sp)
  ld t0, 128(sp)
  mv a0, t0
  li t0, 64
  mv a1, t0
  li t0, 9
  mv a2, t0
  call fill_bytes
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 136(sp)
  ld t0, 136(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 144(sp)
  ld t0, 136(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 152(sp)
  lb t0, 152(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 156(sp)
  lw t0, 156(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then.5
  j test_sbrk_sleep_uptime_if.merge.5
test_sbrk_sleep_uptime_if.then.5:
  call fail
  j test_sbrk_sleep_uptime_if.merge.5
test_sbrk_sleep_uptime_if.merge.5:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 160(sp)
  ld t0, 160(sp)
  li t3, 63
  add t0, t0, t3
  sd t0, 168(sp)
  ld t0, 160(sp)
  li t3, 63
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 176(sp)
  lb t0, 176(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 180(sp)
  lw t0, 180(sp)
  bnez t0, test_sbrk_sleep_uptime_if.then.6
  j test_sbrk_sleep_uptime_if.merge.6
test_sbrk_sleep_uptime_if.then.6:
  call fail
  j test_sbrk_sleep_uptime_if.merge.6
test_sbrk_sleep_uptime_if.merge.6:
  ld ra, 184(sp)
  addi sp, sp, 192
  ret

.section .text
.globl test_fs_calls
test_fs_calls:
  addi sp, sp, -496
  sd ra, 480(sp)
test_fs_calls_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  sd t0, 40(sp)
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 48(sp)
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  sd t0, 56(sp)
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 64(sp)
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  sd t0, 72(sp)
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 80(sp)
  la t0, dir_name
  li t3, 0
  add t0, t0, t3
  sd t0, 88(sp)
  la t0, dir_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 96(sp)
  la t0, dir_name
  li t3, 0
  add t0, t0, t3
  sd t0, 104(sp)
  la t0, dir_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call mkdir
  sw a0, 112(sp)
  lw t0, 112(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 116(sp)
  lw t0, 116(sp)
  bnez t0, test_fs_calls_if.then
  j test_fs_calls_if.merge
test_fs_calls_if.then:
  call fail
  j test_fs_calls_if.merge
test_fs_calls_if.merge:
  la t0, dir_name
  li t3, 0
  add t0, t0, t3
  sd t0, 120(sp)
  la t0, dir_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call chdir
  sw a0, 128(sp)
  lw t0, 128(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 132(sp)
  lw t0, 132(sp)
  bnez t0, test_fs_calls_if.then.1
  j test_fs_calls_if.merge.1
test_fs_calls_if.then.1:
  call fail
  j test_fs_calls_if.merge.1
test_fs_calls_if.merge.1:
  addi t0, sp, 4
  li t3, 0
  add t0, t0, t3
  sd t0, 136(sp)
  addi t0, sp, 4
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 18
  mv a1, t0
  li t0, 12
  mv a2, t0
  call fill_bytes
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  sd t0, 144(sp)
  la t0, O_CREATE
  lw t1, 0(t0)
  sw t1, 152(sp)
  la t0, O_RDWR
  lw t1, 0(t0)
  sw t1, 156(sp)
  lw t0, 152(sp)
  lw t1, 156(sp)
  addw t2, t0, t1
  sw t2, 160(sp)
  la t0, O_TRUNC
  lw t1, 0(t0)
  sw t1, 164(sp)
  lw t0, 160(sp)
  lw t1, 164(sp)
  addw t2, t0, t1
  sw t2, 168(sp)
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  lw t0, 168(sp)
  mv a1, t0
  call open
  sw a0, 172(sp)
  addi t1, sp, 0
  lw t0, 172(sp)
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 176(sp)
  lw t0, 176(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 180(sp)
  lw t0, 180(sp)
  bnez t0, test_fs_calls_if.then.2
  j test_fs_calls_if.merge.2
test_fs_calls_if.then.2:
  call fail
  j test_fs_calls_if.merge.2
test_fs_calls_if.merge.2:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 184(sp)
  addi t0, sp, 4
  li t3, 0
  add t0, t0, t3
  sd t0, 192(sp)
  lw t0, 184(sp)
  mv a0, t0
  addi t0, sp, 4
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 18
  mv a2, t0
  call write
  sw a0, 200(sp)
  lw t0, 200(sp)
  li t1, 18
  xor t2, t0, t1
  snez t2, t2
  sw t2, 204(sp)
  lw t0, 204(sp)
  bnez t0, test_fs_calls_if.then.3
  j test_fs_calls_if.merge.3
test_fs_calls_if.then.3:
  call fail
  j test_fs_calls_if.merge.3
test_fs_calls_if.merge.3:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 208(sp)
  lw t0, 208(sp)
  mv a0, t0
  call close
  sw a0, 212(sp)
  lw t0, 212(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 216(sp)
  lw t0, 216(sp)
  bnez t0, test_fs_calls_if.then.4
  j test_fs_calls_if.merge.4
test_fs_calls_if.then.4:
  call fail
  j test_fs_calls_if.merge.4
test_fs_calls_if.merge.4:
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  sd t0, 224(sp)
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  sd t0, 232(sp)
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call link
  sw a0, 240(sp)
  lw t0, 240(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 244(sp)
  lw t0, 244(sp)
  bnez t0, test_fs_calls_if.then.5
  j test_fs_calls_if.merge.5
test_fs_calls_if.then.5:
  call fail
  j test_fs_calls_if.merge.5
test_fs_calls_if.merge.5:
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  sd t0, 248(sp)
  la t0, O_RDONLY
  lw t1, 0(t0)
  sw t1, 256(sp)
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  lw t0, 256(sp)
  mv a1, t0
  call open
  sw a0, 260(sp)
  addi t1, sp, 0
  lw t0, 260(sp)
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 264(sp)
  lw t0, 264(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 268(sp)
  lw t0, 268(sp)
  bnez t0, test_fs_calls_if.then.6
  j test_fs_calls_if.merge.6
test_fs_calls_if.then.6:
  call fail
  j test_fs_calls_if.merge.6
test_fs_calls_if.merge.6:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 272(sp)
  addi t0, sp, 22
  li t3, 0
  add t0, t0, t3
  sd t0, 280(sp)
  lw t0, 272(sp)
  mv a0, t0
  addi t0, sp, 22
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 18
  mv a2, t0
  call read_full
  sw a0, 288(sp)
  lw t0, 288(sp)
  li t1, 18
  xor t2, t0, t1
  snez t2, t2
  sw t2, 292(sp)
  lw t0, 292(sp)
  bnez t0, test_fs_calls_if.then.7
  j test_fs_calls_if.merge.7
test_fs_calls_if.then.7:
  call fail
  j test_fs_calls_if.merge.7
test_fs_calls_if.merge.7:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 296(sp)
  addi t0, sp, 22
  li t3, 0
  add t0, t0, t3
  sd t0, 304(sp)
  lw t0, 296(sp)
  mv a0, t0
  addi t0, sp, 22
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 1
  mv a2, t0
  call read
  sw a0, 312(sp)
  lw t0, 312(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 316(sp)
  lw t0, 316(sp)
  bnez t0, test_fs_calls_if.then.8
  j test_fs_calls_if.merge.8
test_fs_calls_if.then.8:
  call fail
  j test_fs_calls_if.merge.8
test_fs_calls_if.merge.8:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 320(sp)
  lw t0, 320(sp)
  mv a0, t0
  call close
  sw a0, 324(sp)
  lw t0, 324(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 328(sp)
  lw t0, 328(sp)
  bnez t0, test_fs_calls_if.then.9
  j test_fs_calls_if.merge.9
test_fs_calls_if.then.9:
  call fail
  j test_fs_calls_if.merge.9
test_fs_calls_if.merge.9:
  addi t0, sp, 4
  li t3, 0
  add t0, t0, t3
  sd t0, 336(sp)
  addi t0, sp, 22
  li t3, 0
  add t0, t0, t3
  sd t0, 344(sp)
  addi t0, sp, 4
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  addi t0, sp, 22
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 18
  mv a2, t0
  call same_bytes
  sw a0, 352(sp)
  lw t0, 352(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 356(sp)
  lw t0, 356(sp)
  bnez t0, test_fs_calls_if.then.10
  j test_fs_calls_if.merge.10
test_fs_calls_if.then.10:
  call fail
  j test_fs_calls_if.merge.10
test_fs_calls_if.merge.10:
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  sd t0, 360(sp)
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 1
  mv a1, t0
  li t0, 1
  mv a2, t0
  call mknod
  sw a0, 368(sp)
  lw t0, 368(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 372(sp)
  lw t0, 372(sp)
  bnez t0, test_fs_calls_if.then.11
  j test_fs_calls_if.merge.11
test_fs_calls_if.then.11:
  call fail
  j test_fs_calls_if.merge.11
test_fs_calls_if.merge.11:
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  sd t0, 376(sp)
  la t0, O_RDWR
  lw t1, 0(t0)
  sw t1, 384(sp)
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  lw t0, 384(sp)
  mv a1, t0
  call open
  sw a0, 388(sp)
  addi t1, sp, 0
  lw t0, 388(sp)
  sw t0, 0(t1)
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 392(sp)
  lw t0, 392(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 396(sp)
  lw t0, 396(sp)
  bnez t0, test_fs_calls_if.then.12
  j test_fs_calls_if.merge.12
test_fs_calls_if.then.12:
  call fail
  j test_fs_calls_if.merge.12
test_fs_calls_if.merge.12:
  addi t0, sp, 0
  lw t1, 0(t0)
  sw t1, 400(sp)
  lw t0, 400(sp)
  mv a0, t0
  call close
  sw a0, 404(sp)
  lw t0, 404(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 408(sp)
  lw t0, 408(sp)
  bnez t0, test_fs_calls_if.then.13
  j test_fs_calls_if.merge.13
test_fs_calls_if.then.13:
  call fail
  j test_fs_calls_if.merge.13
test_fs_calls_if.merge.13:
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  sd t0, 416(sp)
  la t0, node_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 424(sp)
  lw t0, 424(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 428(sp)
  lw t0, 428(sp)
  bnez t0, test_fs_calls_if.then.14
  j test_fs_calls_if.merge.14
test_fs_calls_if.then.14:
  call fail
  j test_fs_calls_if.merge.14
test_fs_calls_if.merge.14:
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  sd t0, 432(sp)
  la t0, link_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 440(sp)
  lw t0, 440(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 444(sp)
  lw t0, 444(sp)
  bnez t0, test_fs_calls_if.then.15
  j test_fs_calls_if.merge.15
test_fs_calls_if.then.15:
  call fail
  j test_fs_calls_if.merge.15
test_fs_calls_if.merge.15:
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  sd t0, 448(sp)
  la t0, file_name
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call unlink
  sw a0, 456(sp)
  lw t0, 456(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 460(sp)
  lw t0, 460(sp)
  bnez t0, test_fs_calls_if.then.16
  j test_fs_calls_if.merge.16
test_fs_calls_if.then.16:
  call fail
  j test_fs_calls_if.merge.16
test_fs_calls_if.merge.16:
  la t0, .str.0
  li t3, 0
  add t0, t0, t3
  sd t0, 464(sp)
  la t0, .str.0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call chdir
  sw a0, 472(sp)
  lw t0, 472(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 476(sp)
  lw t0, 476(sp)
  bnez t0, test_fs_calls_if.then.17
  j test_fs_calls_if.merge.17
test_fs_calls_if.then.17:
  call fail
  j test_fs_calls_if.merge.17
test_fs_calls_if.merge.17:
  ld ra, 480(sp)
  addi sp, sp, 496
  ret

.section .text
.globl test_exec
test_exec:
  addi sp, sp, -592
  sd ra, 584(sp)
test_exec_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 64(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  mv a0, t0
  call pipe
  sw a0, 72(sp)
  lw t0, 72(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 76(sp)
  lw t0, 76(sp)
  bnez t0, test_exec_if.then
  j test_exec_if.merge
test_exec_if.then:
  call fail
  j test_exec_if.merge
test_exec_if.merge:
  call fork
  sw a0, 80(sp)
  addi t1, sp, 8
  lw t0, 80(sp)
  sw t0, 0(t1)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 84(sp)
  lw t0, 84(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 88(sp)
  lw t0, 88(sp)
  bnez t0, test_exec_if.then.1
  j test_exec_if.merge.1
test_exec_if.then.1:
  call fail
  j test_exec_if.merge.1
test_exec_if.merge.1:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 92(sp)
  lw t0, 92(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 96(sp)
  lw t0, 96(sp)
  bnez t0, test_exec_if.then.2
  j test_exec_if.merge.2
test_exec_if.then.2:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 104(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 112(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 120(sp)
  lw t0, 120(sp)
  mv a0, t0
  call close
  sw a0, 124(sp)
  lw t0, 124(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 128(sp)
  lw t0, 128(sp)
  bnez t0, test_exec_if.then.3
  j test_exec_if.merge.3
test_exec_if.merge.2:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 136(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 144(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 152(sp)
  lw t0, 152(sp)
  mv a0, t0
  call close
  sw a0, 156(sp)
  lw t0, 156(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 160(sp)
  lw t0, 160(sp)
  bnez t0, test_exec_if.then.7
  j test_exec_if.merge.7
test_exec_if.then.3:
  li t0, 91
  mv a0, t0
  call exit
  j test_exec_if.merge.3
test_exec_if.merge.3:
  li t0, 1
  mv a0, t0
  call close
  sw a0, 164(sp)
  lw t0, 164(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 168(sp)
  lw t0, 168(sp)
  bnez t0, test_exec_if.then.4
  j test_exec_if.merge.4
test_exec_if.then.4:
  li t0, 92
  mv a0, t0
  call exit
  j test_exec_if.merge.4
test_exec_if.merge.4:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 176(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 184(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 192(sp)
  lw t0, 192(sp)
  mv a0, t0
  call dup
  sw a0, 196(sp)
  addi t1, sp, 16
  lw t0, 196(sp)
  sw t0, 0(t1)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 200(sp)
  lw t0, 200(sp)
  li t1, 1
  xor t2, t0, t1
  snez t2, t2
  sw t2, 204(sp)
  lw t0, 204(sp)
  bnez t0, test_exec_if.then.5
  j test_exec_if.merge.5
test_exec_if.then.5:
  li t0, 93
  mv a0, t0
  call exit
  j test_exec_if.merge.5
test_exec_if.merge.5:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 208(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 216(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 224(sp)
  lw t0, 224(sp)
  mv a0, t0
  call close
  sw a0, 228(sp)
  lw t0, 228(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 232(sp)
  lw t0, 232(sp)
  bnez t0, test_exec_if.then.6
  j test_exec_if.merge.6
test_exec_if.then.6:
  li t0, 94
  mv a0, t0
  call exit
  j test_exec_if.merge.6
test_exec_if.merge.6:
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 240(sp)
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 248(sp)
  la t0, echo_prog
  li t3, 0
  add t0, t0, t3
  sd t0, 256(sp)
  addi t1, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t1, t1, t3
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t1, t1, t3
  la t0, echo_prog
  li t3, 0
  add t0, t0, t3
  sd t0, 0(t1)
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 264(sp)
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 272(sp)
  la t0, exec_word
  li t3, 0
  add t0, t0, t3
  sd t0, 280(sp)
  addi t1, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t1, t1, t3
  li t3, 1
  li t4, 8
  mul t3, t3, t4
  add t1, t1, t3
  la t0, exec_word
  li t3, 0
  add t0, t0, t3
  sd t0, 0(t1)
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 288(sp)
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 2
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 296(sp)
  addi t1, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t1, t1, t3
  li t3, 2
  li t4, 8
  mul t3, t3, t4
  add t1, t1, t3
  li t0, 0
  sd t0, 0(t1)
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 304(sp)
  addi t1, sp, 56
  addi t0, sp, 32
  li t3, 0
  li t4, 8
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 0(t1)
  la t0, echo_prog
  li t3, 0
  add t0, t0, t3
  sd t0, 312(sp)
  addi t0, sp, 56
  ld t1, 0(t0)
  sd t1, 320(sp)
  la t0, echo_prog
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  ld t0, 320(sp)
  mv a1, t0
  call exec
  sw a0, 328(sp)
  li t0, 95
  mv a0, t0
  call exit
  j test_exec_if.merge.2
test_exec_if.then.7:
  call fail
  j test_exec_if.merge.7
test_exec_if.merge.7:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 336(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 344(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 352(sp)
  addi t0, sp, 20
  li t3, 0
  add t0, t0, t3
  sd t0, 360(sp)
  lw t0, 352(sp)
  mv a0, t0
  addi t0, sp, 20
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 5
  mv a2, t0
  call read_full
  sw a0, 368(sp)
  lw t0, 368(sp)
  li t1, 5
  xor t2, t0, t1
  snez t2, t2
  sw t2, 372(sp)
  lw t0, 372(sp)
  bnez t0, test_exec_if.then.8
  j test_exec_if.merge.8
test_exec_if.then.8:
  call fail
  j test_exec_if.merge.8
test_exec_if.merge.8:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 376(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 384(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 392(sp)
  addi t0, sp, 20
  li t3, 0
  add t0, t0, t3
  sd t0, 400(sp)
  lw t0, 392(sp)
  mv a0, t0
  addi t0, sp, 20
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 1
  mv a2, t0
  call read
  sw a0, 408(sp)
  lw t0, 408(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 412(sp)
  lw t0, 412(sp)
  bnez t0, test_exec_if.then.9
  j test_exec_if.merge.9
test_exec_if.then.9:
  call fail
  j test_exec_if.merge.9
test_exec_if.merge.9:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 416(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 424(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 432(sp)
  lw t0, 432(sp)
  mv a0, t0
  call close
  sw a0, 436(sp)
  lw t0, 436(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 440(sp)
  lw t0, 440(sp)
  bnez t0, test_exec_if.then.10
  j test_exec_if.merge.10
test_exec_if.then.10:
  call fail
  j test_exec_if.merge.10
test_exec_if.merge.10:
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  sd t0, 448(sp)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  sd t0, 456(sp)
  addi t1, sp, 25
  li t3, 0
  add t1, t1, t3
  li t3, 0
  add t1, t1, t3
  li t0, 69
  sb t0, 0(t1)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  sd t0, 464(sp)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  sd t0, 472(sp)
  addi t1, sp, 25
  li t3, 0
  add t1, t1, t3
  li t3, 1
  add t1, t1, t3
  li t0, 88
  sb t0, 0(t1)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  sd t0, 480(sp)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  sd t0, 488(sp)
  addi t1, sp, 25
  li t3, 0
  add t1, t1, t3
  li t3, 2
  add t1, t1, t3
  li t0, 69
  sb t0, 0(t1)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  sd t0, 496(sp)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  sd t0, 504(sp)
  addi t1, sp, 25
  li t3, 0
  add t1, t1, t3
  li t3, 3
  add t1, t1, t3
  li t0, 67
  sb t0, 0(t1)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  sd t0, 512(sp)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  li t3, 4
  add t0, t0, t3
  sd t0, 520(sp)
  addi t1, sp, 25
  li t3, 0
  add t1, t1, t3
  li t3, 4
  add t1, t1, t3
  li t0, 10
  sb t0, 0(t1)
  addi t0, sp, 20
  li t3, 0
  add t0, t0, t3
  sd t0, 528(sp)
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  sd t0, 536(sp)
  addi t0, sp, 20
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  addi t0, sp, 25
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 5
  mv a2, t0
  call same_bytes
  sw a0, 544(sp)
  lw t0, 544(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 548(sp)
  lw t0, 548(sp)
  bnez t0, test_exec_if.then.11
  j test_exec_if.merge.11
test_exec_if.then.11:
  call fail
  j test_exec_if.merge.11
test_exec_if.merge.11:
  li t0, 0
  li t1, 1
  subw t2, t0, t1
  sw t2, 552(sp)
  addi t1, sp, 12
  lw t0, 552(sp)
  sw t0, 0(t1)
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 556(sp)
  addi t0, sp, 12
  mv a0, t0
  call wait
  sw a0, 560(sp)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 564(sp)
  lw t0, 560(sp)
  lw t1, 564(sp)
  xor t2, t0, t1
  snez t2, t2
  sw t2, 568(sp)
  lw t0, 568(sp)
  bnez t0, test_exec_if.then.12
  j test_exec_if.merge.12
test_exec_if.then.12:
  call fail
  j test_exec_if.merge.12
test_exec_if.merge.12:
  addi t0, sp, 12
  lw t1, 0(t0)
  sw t1, 572(sp)
  lw t0, 572(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 576(sp)
  lw t0, 576(sp)
  bnez t0, test_exec_if.then.13
  j test_exec_if.merge.13
test_exec_if.then.13:
  call fail
  j test_exec_if.merge.13
test_exec_if.merge.13:
  ld ra, 584(sp)
  addi sp, sp, 592
  ret

.section .text
.globl test_string_memory
test_string_memory:
  addi sp, sp, -656
  sd ra, 640(sp)
test_string_memory_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 104(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 0
  mv a1, t0
  li t0, 32
  mv a2, t0
  call memset
  sd a0, 112(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 120(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 0
  mv a1, t0
  li t0, 32
  mv a2, t0
  call memset
  sd a0, 128(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 136(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 0
  mv a1, t0
  li t0, 32
  mv a2, t0
  call memset
  sd a0, 144(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 152(sp)
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  sd t0, 160(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call strcpy
  sd a0, 168(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 176(sp)
  ld t0, 168(sp)
  addi t1, sp, 0
  li t3, 0
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 184(sp)
  lw t0, 184(sp)
  bnez t0, test_string_memory_if.then
  j test_string_memory_if.merge
test_string_memory_if.then:
  call fail
  j test_string_memory_if.merge
test_string_memory_if.merge:
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 192(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call strlen
  sw a0, 200(sp)
  lw t0, 200(sp)
  li t1, 7
  xor t2, t0, t1
  snez t2, t2
  sw t2, 204(sp)
  lw t0, 204(sp)
  bnez t0, test_string_memory_if.then.1
  j test_string_memory_if.merge.1
test_string_memory_if.then.1:
  call fail
  j test_string_memory_if.merge.1
test_string_memory_if.merge.1:
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 208(sp)
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  sd t0, 216(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, .str.1
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call strcmp
  sw a0, 224(sp)
  lw t0, 224(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 228(sp)
  lw t0, 228(sp)
  bnez t0, test_string_memory_if.then.2
  j test_string_memory_if.merge.2
test_string_memory_if.then.2:
  call fail
  j test_string_memory_if.merge.2
test_string_memory_if.merge.2:
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 232(sp)
  la t0, .str.2
  li t3, 0
  add t0, t0, t3
  sd t0, 240(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, .str.2
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call strcmp
  sw a0, 248(sp)
  lw t0, 248(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 252(sp)
  lw t0, 252(sp)
  bnez t0, test_string_memory_if.then.3
  j test_string_memory_if.merge.3
test_string_memory_if.then.3:
  call fail
  j test_string_memory_if.merge.3
test_string_memory_if.merge.3:
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 256(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 264(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 8
  mv a2, t0
  call memcpy
  sd a0, 272(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 280(sp)
  ld t0, 272(sp)
  addi t1, sp, 32
  li t3, 0
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 288(sp)
  lw t0, 288(sp)
  bnez t0, test_string_memory_if.then.4
  j test_string_memory_if.merge.4
test_string_memory_if.then.4:
  call fail
  j test_string_memory_if.merge.4
test_string_memory_if.merge.4:
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  sd t0, 296(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 304(sp)
  addi t0, sp, 0
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 8
  mv a2, t0
  call memcmp
  sw a0, 312(sp)
  lw t0, 312(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 316(sp)
  lw t0, 316(sp)
  bnez t0, test_string_memory_if.then.5
  j test_string_memory_if.merge.5
test_string_memory_if.then.5:
  call fail
  j test_string_memory_if.merge.5
test_string_memory_if.merge.5:
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 320(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 104
  mv a1, t0
  call strchr
  sd a0, 328(sp)
  addi t1, sp, 96
  ld t0, 328(sp)
  sd t0, 0(t1)
  addi t0, sp, 96
  ld t1, 0(t0)
  sd t1, 336(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 344(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  sd t0, 352(sp)
  ld t0, 336(sp)
  addi t1, sp, 32
  li t3, 0
  add t1, t1, t3
  li t3, 3
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 360(sp)
  lw t0, 360(sp)
  bnez t0, test_string_memory_if.then.6
  j test_string_memory_if.merge.6
test_string_memory_if.then.6:
  call fail
  j test_string_memory_if.merge.6
test_string_memory_if.merge.6:
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  sd t0, 368(sp)
  addi t0, sp, 32
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 122
  mv a1, t0
  call strchr
  sd a0, 376(sp)
  ld t0, 376(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 384(sp)
  lw t0, 384(sp)
  bnez t0, test_string_memory_if.then.7
  j test_string_memory_if.merge.7
test_string_memory_if.then.7:
  call fail
  j test_string_memory_if.merge.7
test_string_memory_if.merge.7:
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 392(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 120
  mv a1, t0
  li t0, 10
  mv a2, t0
  call memset
  sd a0, 400(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 408(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 10
  add t0, t0, t3
  sd t0, 416(sp)
  addi t1, sp, 64
  li t3, 0
  add t1, t1, t3
  li t3, 10
  add t1, t1, t3
  li t0, 0
  sb t0, 0(t1)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 424(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  sd t0, 432(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 440(sp)
  lb t0, 440(sp)
  li t1, 120
  xor t2, t0, t1
  snez t2, t2
  sw t2, 444(sp)
  lw t0, 444(sp)
  bnez t0, test_string_memory_if.then.8
  j test_string_memory_if.merge.8
test_string_memory_if.then.8:
  call fail
  j test_string_memory_if.merge.8
test_string_memory_if.merge.8:
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 448(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 9
  add t0, t0, t3
  sd t0, 456(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 9
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 464(sp)
  lb t0, 464(sp)
  li t1, 120
  xor t2, t0, t1
  snez t2, t2
  sw t2, 468(sp)
  lw t0, 468(sp)
  bnez t0, test_string_memory_if.then.9
  j test_string_memory_if.merge.9
test_string_memory_if.then.9:
  call fail
  j test_string_memory_if.merge.9
test_string_memory_if.merge.9:
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 472(sp)
  la t0, .str.3
  li t3, 0
  add t0, t0, t3
  sd t0, 480(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, .str.3
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call strcpy
  sd a0, 488(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 496(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  sd t0, 504(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 512(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  mv a0, t0
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 8
  mv a2, t0
  call memmove
  sd a0, 520(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 528(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  sd t0, 536(sp)
  ld t0, 520(sp)
  addi t1, sp, 64
  li t3, 0
  add t1, t1, t3
  li t3, 2
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 544(sp)
  lw t0, 544(sp)
  bnez t0, test_string_memory_if.then.10
  j test_string_memory_if.merge.10
test_string_memory_if.then.10:
  call fail
  j test_string_memory_if.merge.10
test_string_memory_if.merge.10:
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 552(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  sd t0, 560(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 568(sp)
  lb t0, 568(sp)
  li t1, 48
  xor t2, t0, t1
  snez t2, t2
  sw t2, 572(sp)
  lw t0, 572(sp)
  bnez t0, test_string_memory_if.then.11
  j test_string_memory_if.merge.11
test_string_memory_if.then.11:
  call fail
  j test_string_memory_if.merge.11
test_string_memory_if.merge.11:
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 576(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  sd t0, 584(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 592(sp)
  lb t0, 592(sp)
  li t1, 49
  xor t2, t0, t1
  snez t2, t2
  sw t2, 596(sp)
  lw t0, 596(sp)
  bnez t0, test_string_memory_if.then.12
  j test_string_memory_if.merge.12
test_string_memory_if.then.12:
  call fail
  j test_string_memory_if.merge.12
test_string_memory_if.merge.12:
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  sd t0, 600(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 9
  add t0, t0, t3
  sd t0, 608(sp)
  addi t0, sp, 64
  li t3, 0
  add t0, t0, t3
  li t3, 9
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 616(sp)
  lb t0, 616(sp)
  li t1, 55
  xor t2, t0, t1
  snez t2, t2
  sw t2, 620(sp)
  lw t0, 620(sp)
  bnez t0, test_string_memory_if.then.13
  j test_string_memory_if.merge.13
test_string_memory_if.then.13:
  call fail
  j test_string_memory_if.merge.13
test_string_memory_if.merge.13:
  la t0, .str.4
  li t3, 0
  add t0, t0, t3
  sd t0, 624(sp)
  la t0, .str.4
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  call atoi
  sw a0, 632(sp)
  lw t0, 632(sp)
  li t1, 2048
  xor t2, t0, t1
  snez t2, t2
  sw t2, 636(sp)
  lw t0, 636(sp)
  bnez t0, test_string_memory_if.then.14
  j test_string_memory_if.merge.14
test_string_memory_if.then.14:
  call fail
  j test_string_memory_if.merge.14
test_string_memory_if.merge.14:
  ld ra, 640(sp)
  addi sp, sp, 656
  ret

.section .text
.globl test_printf_fprintf
test_printf_fprintf:
  addi sp, sp, -800
  sd ra, 784(sp)
test_printf_fprintf_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 32(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  mv a0, t0
  call pipe
  sw a0, 40(sp)
  lw t0, 40(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 44(sp)
  lw t0, 44(sp)
  bnez t0, test_printf_fprintf_if.then
  j test_printf_fprintf_if.merge
test_printf_fprintf_if.then:
  call fail
  j test_printf_fprintf_if.merge
test_printf_fprintf_if.merge:
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 48(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  mv a0, t0
  call pipe
  sw a0, 56(sp)
  lw t0, 56(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 60(sp)
  lw t0, 60(sp)
  bnez t0, test_printf_fprintf_if.then.1
  j test_printf_fprintf_if.merge.1
test_printf_fprintf_if.then.1:
  call fail
  j test_printf_fprintf_if.merge.1
test_printf_fprintf_if.merge.1:
  li t0, 1
  mv a0, t0
  call dup
  sw a0, 64(sp)
  addi t1, sp, 16
  lw t0, 64(sp)
  sw t0, 0(t1)
  li t0, 2
  mv a0, t0
  call dup
  sw a0, 68(sp)
  addi t1, sp, 20
  lw t0, 68(sp)
  sw t0, 0(t1)
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 72(sp)
  lw t0, 72(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 76(sp)
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 80(sp)
  lw t0, 80(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 84(sp)
  lw t0, 76(sp)
  lw t1, 84(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 88(sp)
  lw t0, 88(sp)
  bnez t0, test_printf_fprintf_if.then.2
  j test_printf_fprintf_if.merge.2
test_printf_fprintf_if.then.2:
  call fail
  j test_printf_fprintf_if.merge.2
test_printf_fprintf_if.merge.2:
  li t0, 1
  mv a0, t0
  call close
  sw a0, 92(sp)
  lw t0, 92(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 96(sp)
  lw t0, 96(sp)
  bnez t0, test_printf_fprintf_if.then.3
  j test_printf_fprintf_if.merge.3
test_printf_fprintf_if.then.3:
  call fail
  j test_printf_fprintf_if.merge.3
test_printf_fprintf_if.merge.3:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 104(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 112(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 120(sp)
  lw t0, 120(sp)
  mv a0, t0
  call dup
  sw a0, 124(sp)
  lw t0, 124(sp)
  li t1, 1
  xor t2, t0, t1
  snez t2, t2
  sw t2, 128(sp)
  lw t0, 128(sp)
  bnez t0, test_printf_fprintf_if.then.4
  j test_printf_fprintf_if.merge.4
test_printf_fprintf_if.then.4:
  call fail
  j test_printf_fprintf_if.merge.4
test_printf_fprintf_if.merge.4:
  li t0, 2
  mv a0, t0
  call close
  sw a0, 132(sp)
  lw t0, 132(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 136(sp)
  lw t0, 136(sp)
  bnez t0, test_printf_fprintf_if.then.5
  j test_printf_fprintf_if.merge.5
test_printf_fprintf_if.then.5:
  call fail
  j test_printf_fprintf_if.merge.5
test_printf_fprintf_if.merge.5:
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 144(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 152(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 160(sp)
  lw t0, 160(sp)
  mv a0, t0
  call dup
  sw a0, 164(sp)
  lw t0, 164(sp)
  li t1, 2
  xor t2, t0, t1
  snez t2, t2
  sw t2, 168(sp)
  lw t0, 168(sp)
  bnez t0, test_printf_fprintf_if.then.6
  j test_printf_fprintf_if.merge.6
test_printf_fprintf_if.then.6:
  call fail
  j test_printf_fprintf_if.merge.6
test_printf_fprintf_if.merge.6:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 176(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 184(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 192(sp)
  lw t0, 192(sp)
  mv a0, t0
  call close
  sw a0, 196(sp)
  lw t0, 196(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 200(sp)
  lw t0, 200(sp)
  bnez t0, test_printf_fprintf_if.then.7
  j test_printf_fprintf_if.merge.7
test_printf_fprintf_if.then.7:
  call fail
  j test_printf_fprintf_if.merge.7
test_printf_fprintf_if.merge.7:
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 208(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 216(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 224(sp)
  lw t0, 224(sp)
  mv a0, t0
  call close
  sw a0, 228(sp)
  lw t0, 228(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 232(sp)
  lw t0, 232(sp)
  bnez t0, test_printf_fprintf_if.then.8
  j test_printf_fprintf_if.merge.8
test_printf_fprintf_if.then.8:
  call fail
  j test_printf_fprintf_if.merge.8
test_printf_fprintf_if.merge.8:
  la t0, .str.5
  li t3, 0
  add t0, t0, t3
  sd t0, 240(sp)
  la t0, .str.6
  li t3, 0
  add t0, t0, t3
  sd t0, 248(sp)
  la t0, .str.5
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 7
  mv a1, t0
  la t0, .str.6
  li t3, 0
  add t0, t0, t3
  mv a2, t0
  call printf
  la t0, .str.7
  li t3, 0
  add t0, t0, t3
  sd t0, 256(sp)
  la t0, .str.8
  li t3, 0
  add t0, t0, t3
  sd t0, 264(sp)
  li t0, 2
  mv a0, t0
  la t0, .str.7
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 8
  mv a2, t0
  la t0, .str.8
  li t3, 0
  add t0, t0, t3
  mv a3, t0
  call fprintf
  li t0, 1
  mv a0, t0
  call close
  sw a0, 272(sp)
  lw t0, 272(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 276(sp)
  lw t0, 276(sp)
  bnez t0, test_printf_fprintf_if.then.9
  j test_printf_fprintf_if.merge.9
test_printf_fprintf_if.then.9:
  call fail
  j test_printf_fprintf_if.merge.9
test_printf_fprintf_if.merge.9:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 280(sp)
  lw t0, 280(sp)
  mv a0, t0
  call dup
  sw a0, 284(sp)
  lw t0, 284(sp)
  li t1, 1
  xor t2, t0, t1
  snez t2, t2
  sw t2, 288(sp)
  lw t0, 288(sp)
  bnez t0, test_printf_fprintf_if.then.10
  j test_printf_fprintf_if.merge.10
test_printf_fprintf_if.then.10:
  call fail
  j test_printf_fprintf_if.merge.10
test_printf_fprintf_if.merge.10:
  addi t0, sp, 16
  lw t1, 0(t0)
  sw t1, 292(sp)
  lw t0, 292(sp)
  mv a0, t0
  call close
  sw a0, 296(sp)
  lw t0, 296(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 300(sp)
  lw t0, 300(sp)
  bnez t0, test_printf_fprintf_if.then.11
  j test_printf_fprintf_if.merge.11
test_printf_fprintf_if.then.11:
  call fail
  j test_printf_fprintf_if.merge.11
test_printf_fprintf_if.merge.11:
  li t0, 2
  mv a0, t0
  call close
  sw a0, 304(sp)
  lw t0, 304(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 308(sp)
  lw t0, 308(sp)
  bnez t0, test_printf_fprintf_if.then.12
  j test_printf_fprintf_if.merge.12
test_printf_fprintf_if.then.12:
  call fail
  j test_printf_fprintf_if.merge.12
test_printf_fprintf_if.merge.12:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 312(sp)
  lw t0, 312(sp)
  mv a0, t0
  call dup
  sw a0, 316(sp)
  lw t0, 316(sp)
  li t1, 2
  xor t2, t0, t1
  snez t2, t2
  sw t2, 320(sp)
  lw t0, 320(sp)
  bnez t0, test_printf_fprintf_if.then.13
  j test_printf_fprintf_if.merge.13
test_printf_fprintf_if.then.13:
  call fail
  j test_printf_fprintf_if.merge.13
test_printf_fprintf_if.merge.13:
  addi t0, sp, 20
  lw t1, 0(t0)
  sw t1, 324(sp)
  lw t0, 324(sp)
  mv a0, t0
  call close
  sw a0, 328(sp)
  lw t0, 328(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 332(sp)
  lw t0, 332(sp)
  bnez t0, test_printf_fprintf_if.then.14
  j test_printf_fprintf_if.merge.14
test_printf_fprintf_if.then.14:
  call fail
  j test_printf_fprintf_if.merge.14
test_printf_fprintf_if.merge.14:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 336(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 344(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 352(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  sd t0, 360(sp)
  lw t0, 352(sp)
  mv a0, t0
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 4
  mv a2, t0
  call read_full
  sw a0, 368(sp)
  lw t0, 368(sp)
  li t1, 4
  xor t2, t0, t1
  snez t2, t2
  sw t2, 372(sp)
  lw t0, 372(sp)
  bnez t0, test_printf_fprintf_if.then.15
  j test_printf_fprintf_if.merge.15
test_printf_fprintf_if.then.15:
  call fail
  j test_printf_fprintf_if.merge.15
test_printf_fprintf_if.merge.15:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 376(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 384(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 392(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  sd t0, 400(sp)
  lw t0, 392(sp)
  mv a0, t0
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 1
  mv a2, t0
  call read
  sw a0, 408(sp)
  lw t0, 408(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 412(sp)
  lw t0, 412(sp)
  bnez t0, test_printf_fprintf_if.then.16
  j test_printf_fprintf_if.merge.16
test_printf_fprintf_if.then.16:
  call fail
  j test_printf_fprintf_if.merge.16
test_printf_fprintf_if.merge.16:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 416(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 424(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 432(sp)
  lw t0, 432(sp)
  mv a0, t0
  call close
  sw a0, 436(sp)
  lw t0, 436(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 440(sp)
  lw t0, 440(sp)
  bnez t0, test_printf_fprintf_if.then.17
  j test_printf_fprintf_if.merge.17
test_printf_fprintf_if.then.17:
  call fail
  j test_printf_fprintf_if.merge.17
test_printf_fprintf_if.merge.17:
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 448(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 456(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 464(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  sd t0, 472(sp)
  lw t0, 464(sp)
  mv a0, t0
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 4
  mv a2, t0
  call read_full
  sw a0, 480(sp)
  lw t0, 480(sp)
  li t1, 4
  xor t2, t0, t1
  snez t2, t2
  sw t2, 484(sp)
  lw t0, 484(sp)
  bnez t0, test_printf_fprintf_if.then.18
  j test_printf_fprintf_if.merge.18
test_printf_fprintf_if.then.18:
  call fail
  j test_printf_fprintf_if.merge.18
test_printf_fprintf_if.merge.18:
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 488(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 496(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 504(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  sd t0, 512(sp)
  lw t0, 504(sp)
  mv a0, t0
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 1
  mv a2, t0
  call read
  sw a0, 520(sp)
  lw t0, 520(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 524(sp)
  lw t0, 524(sp)
  bnez t0, test_printf_fprintf_if.then.19
  j test_printf_fprintf_if.merge.19
test_printf_fprintf_if.then.19:
  call fail
  j test_printf_fprintf_if.merge.19
test_printf_fprintf_if.merge.19:
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 528(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 536(sp)
  addi t0, sp, 8
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 544(sp)
  lw t0, 544(sp)
  mv a0, t0
  call close
  sw a0, 548(sp)
  lw t0, 548(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 552(sp)
  lw t0, 552(sp)
  bnez t0, test_printf_fprintf_if.then.20
  j test_printf_fprintf_if.merge.20
test_printf_fprintf_if.then.20:
  call fail
  j test_printf_fprintf_if.merge.20
test_printf_fprintf_if.merge.20:
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  sd t0, 560(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  sd t0, 568(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 576(sp)
  lb t0, 576(sp)
  li t1, 80
  xor t2, t0, t1
  snez t2, t2
  sw t2, 580(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  sd t0, 584(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  sd t0, 592(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 600(sp)
  lb t0, 600(sp)
  li t1, 55
  xor t2, t0, t1
  snez t2, t2
  sw t2, 604(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  sd t0, 608(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  sd t0, 616(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 624(sp)
  lb t0, 624(sp)
  li t1, 81
  xor t2, t0, t1
  snez t2, t2
  sw t2, 628(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  sd t0, 632(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  sd t0, 640(sp)
  addi t0, sp, 24
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 648(sp)
  lb t0, 648(sp)
  li t1, 10
  xor t2, t0, t1
  snez t2, t2
  sw t2, 652(sp)
  lw t0, 628(sp)
  lw t1, 652(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 656(sp)
  lw t0, 604(sp)
  lw t1, 656(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 660(sp)
  lw t0, 580(sp)
  lw t1, 660(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 664(sp)
  lw t0, 664(sp)
  bnez t0, test_printf_fprintf_if.then.21
  j test_printf_fprintf_if.merge.21
test_printf_fprintf_if.then.21:
  call fail
  j test_printf_fprintf_if.merge.21
test_printf_fprintf_if.merge.21:
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  sd t0, 672(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  sd t0, 680(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 688(sp)
  lb t0, 688(sp)
  li t1, 69
  xor t2, t0, t1
  snez t2, t2
  sw t2, 692(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  sd t0, 696(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  sd t0, 704(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 1
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 712(sp)
  lb t0, 712(sp)
  li t1, 56
  xor t2, t0, t1
  snez t2, t2
  sw t2, 716(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  sd t0, 720(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  sd t0, 728(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 2
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 736(sp)
  lb t0, 736(sp)
  li t1, 82
  xor t2, t0, t1
  snez t2, t2
  sw t2, 740(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  sd t0, 744(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  sd t0, 752(sp)
  addi t0, sp, 28
  li t3, 0
  add t0, t0, t3
  li t3, 3
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 760(sp)
  lb t0, 760(sp)
  li t1, 10
  xor t2, t0, t1
  snez t2, t2
  sw t2, 764(sp)
  lw t0, 740(sp)
  lw t1, 764(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 768(sp)
  lw t0, 716(sp)
  lw t1, 768(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 772(sp)
  lw t0, 692(sp)
  lw t1, 772(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 776(sp)
  lw t0, 776(sp)
  bnez t0, test_printf_fprintf_if.then.22
  j test_printf_fprintf_if.merge.22
test_printf_fprintf_if.then.22:
  call fail
  j test_printf_fprintf_if.merge.22
test_printf_fprintf_if.merge.22:
  ld ra, 784(sp)
  addi sp, sp, 800
  ret

.section .text
.globl test_gets
test_gets:
  addi sp, sp, -304
  sd ra, 296(sp)
test_gets_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 32(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  mv a0, t0
  call pipe
  sw a0, 40(sp)
  lw t0, 40(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 44(sp)
  lw t0, 44(sp)
  bnez t0, test_gets_if.then
  j test_gets_if.merge
test_gets_if.then:
  call fail
  j test_gets_if.merge
test_gets_if.merge:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 48(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 56(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 64(sp)
  la t0, .str.9
  li t3, 0
  add t0, t0, t3
  sd t0, 72(sp)
  lw t0, 64(sp)
  mv a0, t0
  la t0, .str.9
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 5
  mv a2, t0
  call write
  sw a0, 80(sp)
  lw t0, 80(sp)
  li t1, 5
  xor t2, t0, t1
  snez t2, t2
  sw t2, 84(sp)
  lw t0, 84(sp)
  bnez t0, test_gets_if.then.1
  j test_gets_if.merge.1
test_gets_if.then.1:
  call fail
  j test_gets_if.merge.1
test_gets_if.merge.1:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 88(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 96(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 1
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 104(sp)
  lw t0, 104(sp)
  mv a0, t0
  call close
  sw a0, 108(sp)
  lw t0, 108(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 112(sp)
  lw t0, 112(sp)
  bnez t0, test_gets_if.then.2
  j test_gets_if.merge.2
test_gets_if.then.2:
  call fail
  j test_gets_if.merge.2
test_gets_if.merge.2:
  li t0, 0
  mv a0, t0
  call dup
  sw a0, 116(sp)
  addi t1, sp, 8
  lw t0, 116(sp)
  sw t0, 0(t1)
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 120(sp)
  lw t0, 120(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 124(sp)
  lw t0, 124(sp)
  bnez t0, test_gets_if.then.3
  j test_gets_if.merge.3
test_gets_if.then.3:
  call fail
  j test_gets_if.merge.3
test_gets_if.merge.3:
  li t0, 0
  mv a0, t0
  call close
  sw a0, 128(sp)
  lw t0, 128(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 132(sp)
  lw t0, 132(sp)
  bnez t0, test_gets_if.then.4
  j test_gets_if.merge.4
test_gets_if.then.4:
  call fail
  j test_gets_if.merge.4
test_gets_if.merge.4:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 136(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 144(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 152(sp)
  lw t0, 152(sp)
  mv a0, t0
  call dup
  sw a0, 156(sp)
  lw t0, 156(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 160(sp)
  lw t0, 160(sp)
  bnez t0, test_gets_if.then.5
  j test_gets_if.merge.5
test_gets_if.then.5:
  call fail
  j test_gets_if.merge.5
test_gets_if.merge.5:
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 168(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  sd t0, 176(sp)
  addi t0, sp, 0
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  li t3, 0
  li t4, 4
  mul t3, t3, t4
  add t0, t0, t3
  lw t1, 0(t0)
  sw t1, 184(sp)
  lw t0, 184(sp)
  mv a0, t0
  call close
  sw a0, 188(sp)
  lw t0, 188(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 192(sp)
  lw t0, 192(sp)
  bnez t0, test_gets_if.then.6
  j test_gets_if.merge.6
test_gets_if.then.6:
  call fail
  j test_gets_if.merge.6
test_gets_if.merge.6:
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 200(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  li t0, 12
  mv a1, t0
  call gets
  sd a0, 208(sp)
  addi t1, sp, 24
  ld t0, 208(sp)
  sd t0, 0(t1)
  li t0, 0
  mv a0, t0
  call close
  sw a0, 216(sp)
  lw t0, 216(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 220(sp)
  lw t0, 220(sp)
  bnez t0, test_gets_if.then.7
  j test_gets_if.merge.7
test_gets_if.then.7:
  call fail
  j test_gets_if.merge.7
test_gets_if.merge.7:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 224(sp)
  lw t0, 224(sp)
  mv a0, t0
  call dup
  sw a0, 228(sp)
  lw t0, 228(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 232(sp)
  lw t0, 232(sp)
  bnez t0, test_gets_if.then.8
  j test_gets_if.merge.8
test_gets_if.then.8:
  call fail
  j test_gets_if.merge.8
test_gets_if.merge.8:
  addi t0, sp, 8
  lw t1, 0(t0)
  sw t1, 236(sp)
  lw t0, 236(sp)
  mv a0, t0
  call close
  sw a0, 240(sp)
  lw t0, 240(sp)
  li t1, 0
  slt t2, t0, t1
  sw t2, 244(sp)
  lw t0, 244(sp)
  bnez t0, test_gets_if.then.9
  j test_gets_if.merge.9
test_gets_if.then.9:
  call fail
  j test_gets_if.merge.9
test_gets_if.merge.9:
  addi t0, sp, 24
  ld t1, 0(t0)
  sd t1, 248(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 256(sp)
  ld t0, 248(sp)
  addi t1, sp, 12
  li t3, 0
  add t1, t1, t3
  xor t2, t0, t1
  snez t2, t2
  sw t2, 264(sp)
  lw t0, 264(sp)
  bnez t0, test_gets_if.then.10
  j test_gets_if.merge.10
test_gets_if.then.10:
  call fail
  j test_gets_if.merge.10
test_gets_if.merge.10:
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  sd t0, 272(sp)
  la t0, .str.9
  li t3, 0
  add t0, t0, t3
  sd t0, 280(sp)
  addi t0, sp, 12
  li t3, 0
  add t0, t0, t3
  mv a0, t0
  la t0, .str.9
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  call strcmp
  sw a0, 288(sp)
  lw t0, 288(sp)
  li t1, 0
  xor t2, t0, t1
  snez t2, t2
  sw t2, 292(sp)
  lw t0, 292(sp)
  bnez t0, test_gets_if.then.11
  j test_gets_if.merge.11
test_gets_if.then.11:
  call fail
  j test_gets_if.merge.11
test_gets_if.merge.11:
  ld ra, 296(sp)
  addi sp, sp, 304
  ret

.section .text
.globl test_malloc_free
test_malloc_free:
  addi sp, sp, -272
  sd ra, 264(sp)
test_malloc_free_entry:
  # alloc lowered to stack slot during frame layout
  # alloc lowered to stack slot during frame layout
  li t0, 32
  mv a0, t0
  call malloc
  sd a0, 16(sp)
  addi t1, sp, 0
  ld t0, 16(sp)
  sd t0, 0(t1)
  li t0, 48
  mv a0, t0
  call malloc
  sd a0, 24(sp)
  addi t1, sp, 8
  ld t0, 24(sp)
  sd t0, 0(t1)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 32(sp)
  ld t0, 32(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 40(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 48(sp)
  ld t0, 48(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 56(sp)
  lw t0, 40(sp)
  lw t1, 56(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 60(sp)
  lw t0, 60(sp)
  bnez t0, test_malloc_free_if.then
  j test_malloc_free_if.merge
test_malloc_free_if.then:
  call fail
  j test_malloc_free_if.merge
test_malloc_free_if.merge:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 64(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 72(sp)
  ld t0, 64(sp)
  ld t1, 72(sp)
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 80(sp)
  lw t0, 80(sp)
  bnez t0, test_malloc_free_if.then.1
  j test_malloc_free_if.merge.1
test_malloc_free_if.then.1:
  call fail
  j test_malloc_free_if.merge.1
test_malloc_free_if.merge.1:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 88(sp)
  ld t0, 88(sp)
  mv a0, t0
  li t0, 32
  mv a1, t0
  li t0, 3
  mv a2, t0
  call fill_bytes
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 96(sp)
  ld t0, 96(sp)
  mv a0, t0
  li t0, 48
  mv a1, t0
  li t0, 9
  mv a2, t0
  call fill_bytes
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 104(sp)
  ld t0, 104(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 112(sp)
  ld t0, 104(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 120(sp)
  lb t0, 120(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 124(sp)
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 128(sp)
  ld t0, 128(sp)
  li t3, 31
  add t0, t0, t3
  sd t0, 136(sp)
  ld t0, 128(sp)
  li t3, 31
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 144(sp)
  lb t0, 144(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 148(sp)
  lw t0, 124(sp)
  lw t1, 148(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 152(sp)
  lw t0, 152(sp)
  bnez t0, test_malloc_free_if.then.2
  j test_malloc_free_if.merge.2
test_malloc_free_if.then.2:
  call fail
  j test_malloc_free_if.merge.2
test_malloc_free_if.merge.2:
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 160(sp)
  ld t0, 160(sp)
  li t3, 0
  add t0, t0, t3
  sd t0, 168(sp)
  ld t0, 160(sp)
  li t3, 0
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 176(sp)
  lb t0, 176(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 180(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 184(sp)
  ld t0, 184(sp)
  li t3, 47
  add t0, t0, t3
  sd t0, 192(sp)
  ld t0, 184(sp)
  li t3, 47
  add t0, t0, t3
  lb t1, 0(t0)
  sb t1, 200(sp)
  lb t0, 200(sp)
  li t1, 0
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 204(sp)
  lw t0, 180(sp)
  lw t1, 204(sp)
  or t2, t0, t1
  addiw t2, t2, 0
  sw t2, 208(sp)
  lw t0, 208(sp)
  bnez t0, test_malloc_free_if.then.3
  j test_malloc_free_if.merge.3
test_malloc_free_if.then.3:
  call fail
  j test_malloc_free_if.merge.3
test_malloc_free_if.merge.3:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 216(sp)
  ld t0, 216(sp)
  mv a0, t0
  li t0, 32
  mv a1, t0
  call sum_bytes
  sw a0, 224(sp)
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 232(sp)
  ld t0, 232(sp)
  mv a0, t0
  li t0, 32
  mv a1, t0
  call sum_bytes
  sw a0, 240(sp)
  lw t0, 224(sp)
  lw t1, 240(sp)
  xor t2, t0, t1
  seqz t2, t2
  sw t2, 244(sp)
  lw t0, 244(sp)
  bnez t0, test_malloc_free_if.then.4
  j test_malloc_free_if.merge.4
test_malloc_free_if.then.4:
  call fail
  j test_malloc_free_if.merge.4
test_malloc_free_if.merge.4:
  addi t0, sp, 0
  ld t1, 0(t0)
  sd t1, 248(sp)
  ld t0, 248(sp)
  mv a0, t0
  call free
  addi t0, sp, 8
  ld t1, 0(t0)
  sd t1, 256(sp)
  ld t0, 256(sp)
  mv a0, t0
  call free
  ld ra, 264(sp)
  addi sp, sp, 272
  ret

.section .text
.globl main
main:
  addi sp, sp, -32
  sd ra, 16(sp)
main_entry:
  call test_basic_proc
  call test_kill
  call test_pipe_read_write_close_dup
  call test_sbrk_sleep_uptime
  call test_fs_calls
  call test_exec
  call test_string_memory
  call test_printf_fprintf
  call test_gets
  call test_malloc_free
  la t0, ok_msg
  li t3, 0
  add t0, t0, t3
  sd t0, 0(sp)
  li t0, 1
  mv a0, t0
  la t0, ok_msg
  li t3, 0
  add t0, t0, t3
  mv a1, t0
  li t0, 16
  mv a2, t0
  call write
  sw a0, 8(sp)
  li t0, 0
  mv a0, t0
  call exit
  li a0, 0
  ld ra, 16(sp)
  addi sp, sp, 32
  ret
