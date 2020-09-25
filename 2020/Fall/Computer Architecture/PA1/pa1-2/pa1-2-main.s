##################################################
#SWE3005 Introduction to Computer Architectures (Fall 2020)

#Instructor: Prof. Euiseong Seo (euiseong@skku.edu)

#TA: Junyeol Yu(ryuwhale95@g.skku.edu), Donggyu Choi(gm03003@gmail.com)

#Description: Print out Greatest Common Divisor (GCD) and Least Common Multiple (LCM)

#Copyright (c) 2020 SungKyunKwan Univ. CSI(Computer Systems Intelligence)
##################################################
.data
  buffer:   .space 1000
  A:        .space 400
  B:        .space 400
  C:        .space 400
  DIM:      .space 4
  N:        .space 4
  input:	  .asciiz "pa1-2.input"
  ENTER:	  .asciiz "\n"
  SPACE:	  .asciiz "\t"

.text
main:
  addi	$sp, $sp, -4
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
  move  $a0, $s0
  la	$a1, buffer
  li	$a2, 1000
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
  move  $t0, $0                 # DIM = 0
  move  $t1, $0                 # N = 0
  lw    $s0, 4($sp)             # &buffer[cursor]
  li    $s1, 32                 # $s1 = ' '   (space)
  li    $s2, 10                 # $s2 = '\n'  (newline)
  li    $s3, 10                 # $s3 = digit (10)

WHILELOOP1:
  lb    $t2, 0($s0)             # buffer[cursor]
  addi  $s0, $s0, 1             # cursor++
  beq   $t2, $s1, WHILELOOP2    # buffer[cursor] == ' '?
  beq   $t2, $s2, WHILELOOP2    # buffer[cursor] == '\n'?
  mul   $t0, $t0, $s3           # n *= 10
  add   $t0, $t0, $t2
  addi  $t0, $t0, -48           # n += buffer[cursor] - '0'
  j     WHILELOOP1
WHILELOOP2:
  lb    $t2, 0($s0)             # buffer[cursor]
  addi  $s0, $s0, 1             # cursor++
  beq   $t2, $s1, ENDLOOP       # buffer[cursor] == ' '?
  beq   $t2, $s2, ENDLOOP       # buffer[cursor] == '\n'?
  mul   $t1, $t1, $s3           # m *= 10
  add   $t1, $t1, $t2
  addi  $t1, $t1, -48           # m += buffer[cursor] - '0'
  j     WHILELOOP2
ENDLOOP:
  # save DIM, N
  la $t2, DIM                   # $t2 = &DIM
  sw $t0, 0($t2)                # MEM[$t2+0] = $t0(DIM)
  la $t2, N                     # $t2 = &N
  sw $t1, 0($t2)                # MEM[$t2+0] = $t1(N)

  move  $t0, $0                 # i = 0
  la $t3, DIM                   # &DIM
  lw $t2, 0($t3)                # $t2 = DIM
  la $s4, A                     # &DIM
  la $s5, B                     # &DIM
  move $t4, $0                  # v = 0
FORLOOP1:
  slt $t3, $t0, $t2             # i < DIM
  beq $t3, $0, FOREND1
  move  $t1, $0                 # j = 0
FORLOOP2:
  slt $t3, $t1, $t2             # j < DIM
  beq $t3, $0, FOREND2
WHILELOOP3:
  lb    $t3, 0($s0)             # buffer[cursor]
  addi  $s0, $s0, 1             # cursor++
  beq   $t3, $s1, WHILEEND      # buffer[cursor] == ' '?
  beq   $t3, $s2, WHILEEND      # buffer[cursor] == '\n'?
  mul   $t4, $t4, $s3           # v *= 10
  add   $t4, $t4, $t3
  addi  $t4, $t4, -48           # v += buffer[cursor] - '0'
  j     WHILELOOP3
WHILEEND:
  # A[i][j] = v
  addi $t5, $0, 4
  mul $t5, $t5, $t1
  addi $t6, $0, 40
  mul $t6, $t6, $t0
  add $t5, $t5, $t6
  add $t5, $t5, $s4
  sw $t4, 0($t5)

  # B[i][j] = v
  addi $t5, $0, 4
  mul $t5, $t5, $t1
  addi $t6, $0, 40
  mul $t6, $t6, $t0
  add $t5, $t5, $t6
  add $t5, $t5, $s5
  sw $t4, 0($t5)
  move $t4, $0                  # v = 0
  addi $t1, $t1, 1              # j++
  j FORLOOP2
FOREND2:
  addi $t0, $t0, 1              # i++
  j FORLOOP1
FOREND1:
  # call product
  la  $t0, N                    # $t0 = &N
  lw  $a0, 0($t0)               # arg1: N
  jal product

  # call printResult
  jal printResult

  # return
  lw    $ra, 0($sp)
  addi  $sp, $sp, 4
  jr    $ra
