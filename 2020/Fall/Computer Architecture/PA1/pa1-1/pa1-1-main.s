##################################################
#SWE3005 Introduction to Computer Architectures (Fall 2020)

#Instructor: Prof. Euiseong Seo (euiseong@skku.edu)

#TA: Junyeol Yu(ryuwhale95@g.skku.edu), Donggyu Choi(gm03003@gmail.com)

#Description: Print out Greatest Common Divisor (GCD) and Least Common Multiple (LCM)

#Copyright (c) 2020 SungKyunKwan Univ. CSI(Computer Systems Intelligence)
##################################################
.data
  buffer:	  .space 80
  input:	  .asciiz "pa1-1.input"
  output1:	.asciiz "GCD between "
  output2:	.asciiz "LCM between "
  output3:  .asciiz " and "
  output4:  .asciiz " is "
  ENTER:	  .asciiz "\n"
  SPACE:	  .asciiz " "

.text
main:
  addi	$sp, $sp, -24
  sw	$ra, 0($sp)

  # open input file
  li	$v0, 13
  la	$a0, input
  la	$a1, 0x000
  la	$a2, 0644
  syscall
  move $s0, $v0
  #############

  # read input file
  li	$v0, 14
  move $a0, $s0
  la	$a1, buffer
  li	$a2, 80
  syscall
  #############

  add   $s1, $a1, $v0
  addi  $s2, $s1, -1
  li    $s3, 10
  lb    $s4, 0($s2)
  beq   $s3, $s4, LNX
WIN:
  sb    $s3, 0($s1)
  sb    $0, 1($s1)
  j     MER
LNX:
  sb    $0, 0($s1)
MER:

  # close input file
  li	$v0, 16
  move $a0, $s0
  syscall
  #############

INIT:
  #  init  #
  #  0($sp) =  $ra
  #  4($sp) =  &buffer[0]
  la    $t0, buffer     
  sw    $t0, 4($sp)

START:
  #  start  #
  move  $t0, $0               # n = 0
  move  $t1, $0               # m = 0
  lw    $t2, 4($sp)           # &buffer[cursor]
  li    $t3, 32               # $t4 = ' '   (space)
  li    $t4, 10               # $t5 = '\n'  (newline)
  li    $t5, 10               # $t6 = digit (10)

WHILELOOP1:
  lb    $t6, 0($t2)           # buffer[cursor]
  addi  $t2, $t2, 1           # cursor++
  beq   $t6, $t3, WHILELOOP2  # buffer[cursor] == ' '?
  beq   $t6, $t4, WHILELOOP2  # buffer[cursor] == '\n'?
  mul   $t0, $t0, $t5         # n *= 10
  add   $t0, $t0, $t6
  addi  $t0, $t0, -48         # n += buffer[cursor] - '0'
  j     WHILELOOP1
WHILELOOP2:
  lb    $t6, 0($t2)           # buffer[cursor]
  addi  $t2, $t2, 1           # cursor++
  beq   $t6, $t3, ENDLOOP     # buffer[cursor] == ' '?
  beq   $t6, $t4, ENDLOOP     # buffer[cursor] == '\n'?
  mul   $t1, $t1, $t5         # m *= 10
  add   $t1, $t1, $t6
  addi  $t1, $t1, -48         # m += buffer[cursor] - '0'
  j     WHILELOOP2
ENDLOOP:
  # save n, m
  sw    $t0, 8($sp)
  sw    $t1, 12($sp)

  # call GCD
  lw  $a0, 8($sp)             # arg1: n
  lw  $a1, 12($sp)            # arg2: m
  jal   GCD
  sw  $v0, 16($sp)            # $v0 = Function GCD's return value

  # call LCM
  lw  $a0, 8($sp)             # arg1: n
  lw  $a1, 12($sp)            # arg2: m
  jal   LCM
  sw  $v0, 20($sp)            # $v0 = Function LCM's return value

  # print result
  li	$v0, 4
  la	$a0, output1
  syscall		                  # printf("GCD between ")
  li	$v0, 1
  lw	 $a0, 8($sp)
  syscall		                  # printf("%d", n)
  li	$v0, 4
  la	$a0, output3
  syscall		                  # printf(" and ")
  li	$v0, 1
  lw	 $a0, 12($sp)
  syscall		                  # printf("%d", m)
  li	$v0, 4
  la	$a0, output4
  syscall		                  # printf(" is ")
  li	$v0, 1
  lw	 $a0, 16($sp)
  syscall		                  # printf("%d", G)
  li	$v0, 4
  la	$a0, ENTER
  syscall		                  # printf("\n")

  li	$v0, 4
  la	$a0, output2
  syscall		                  # printf("LCM between ")
  li	$v0, 1
  lw	 $a0, 8($sp)
  syscall		                  # printf("%d", n)
  li	$v0, 4
  la	$a0, output3
  syscall		                  # printf(" and ")
  li	$v0, 1
  lw	 $a0, 12($sp)
  syscall		                  # printf("%d", m)
  li	$v0, 4
  la	$a0, output4
  syscall		                  # printf(" is ")
  li	$v0, 1
  lw	 $a0, 20($sp)
  syscall		                  # printf("%d", L)
  li	$v0, 4
  la	$a0, ENTER
  syscall		                  # printf("\n")

  # return
  lw    $ra, 0($sp)
  addi  $sp, $sp, 24
  jr    $ra
