product:
#-----------------------------------------------------
# Your code "START" here
#-----------------------------------------------------
  addi $t0, $0, 1
  beq $a0, $t0, ENDPRODUCT

  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $a0, 0($sp)

  la $t1, DIM
  lw $t0, 0($t1)                # t0 <- DIM
  move $t1, $0                  # i = 0
  j FIRSTLOOP2

FIRSTLOOP1:
  addi $t1, $t1, 1              # i++
FIRSTLOOP2:
  beq $t1, $t0, ENDFIRSTLOOP    # i == DIM
  move $t2, $0                  # j = 0
  j SECONDLOOP2
SECONDLOOP1:
  addi $t2, $t2, 1              # j++
SECONDLOOP2:
  beq $t2, $t0, FIRSTLOOP1      # j == DIM
  move $t3, $0                  # k = 0
  j THIRDLOOP2
THIRDLOOP1:
  addi $t3, $t3, 1              # k++
THIRDLOOP2:
  beq $t3, $t0, SECONDLOOP1     # k == DIM

  addi $t4, $0, 10
  mul $t4, $t1, $t4             # 10 * i
  add $t4, $t4, $t3             # 10 * i + k
  sll $t4, $t4, 2               # 4(10 * i + k)
  la $t5, A                     # A
  add $t5, $t5, $t4             # A + 4(10 * i + k)
  lw $t6, 0($t5)                # A[i][k]

  addi $t4, $0, 10
  mul $t4, $t3, $t4             # 10 * k
  add $t4, $t4, $t2             # 10 * k + j
  sll $t4, $t4, 2               # 4(10 * k + j)
  la $t5, B                     # B
  add $t5, $t5, $t4             # B + 4(10 * k + j)
  lw $t7, 0($t5)                # B[k][j]

  mul $t6, $t6, $t7             # A[i][k] * B[k][j]
  
  addi $t4, $0, 10
  mul $t4, $t1, $t4             # 10 * i
  add $t4, $t4, $t2             # 10 * i + j
  sll $t4, $t4, 2               # 4(10 * i + j)
  la $t5, C                     # C
  add $t5, $t5, $t4             # C + 4(10 * i + j)
  lw $t7, 0($t5)                # C[i][j]

  add $t7, $t7, $t6             # C[i][j] += A[i][k] * B[k][j]
  sw $t7, 0($t5)

  j THIRDLOOP1

ENDFIRSTLOOP:
  move $t1, $0                  # i = 0
  j L2

L1:
  addi $t1, $t1, 1              # i++
L2:
  beq $t1, $t0, L5              # i == DIM
  move $t2, $0                  # j = 0
  j L4
L3:
  addi $t2, $t2, 1              # j++
L4:
  beq $t2, $t0, L1              # j == DIM

  addi $t4, $0, 10
  mul $t4, $t1, $t4             # 10 * i
  add $t4, $t4, $t2             # 10 * i + j
  sll $t4, $t4, 2               # 4(10 * i + j)
  la $t5, C                     # C
  add $t5, $t5, $t4             # C + 4(10 * i + j)
  lw $t7, 0($t5)                # C[i][j]

  la $t6, B                     # B
  add $t6, $t6, $t4             # B + 4(10 * i + j)
  sw $t7, 0($t6)                # B[i][j] = C[i][j]
  sw $0, 0($t5)                 # C[i][j] = 0

  j L3

L5:
  addi $a0, $a0, -1             # n - 1
  jal product                   # recursive call

  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8

ENDPRODUCT:
#-----------------------------------------------------
# Your code "END" here
#-----------------------------------------------------
jr  $ra

printResult:
#-----------------------------------------------------
# Your code "START" here
#-----------------------------------------------------
  addi $sp, $sp, -12
  sw $s0, 8($sp)
  sw $s1, 4($sp)
  sw $s2, 0($sp)

  move $s0, $0                  # i = 0
  la $t0, DIM
  lw $s2, 0($t0)                # s2 <- DIM
  j OUTERLOOP2

OUTERLOOP1:
  li $v0, 4
  la $a0, ENTER
  syscall		                # printf("\n")

  addi $s0, $s0, 1              # i++
OUTERLOOP2:
  beq $s0, $s2, ENDOUTERLOOP    # i == DIM
  move $s1, $0                  # j = 0
  j INNERLOOP2                  
INNERLOOP1:
  addi $s1, $s1, 1              # j++
INNERLOOP2:
  beq $s1, $s2, OUTERLOOP1      # j == DIM

  addi $t0, $0, 10
  mul $t0, $t0, $s0             # 10 * i
  add $t0, $t0, $s1             # 10 * i + j
  sll $t0, $t0, 2               # 4(10 * i + j)
  la $t1, B                     # B
  add $t0, $t0, $t1             # B + 4(10 * i + j)

  li $v0, 1
  lw $a0, 0($t0)
  syscall		                # printf("%d", B[i][j])
  li $v0, 4
  la $a0, SPACE
  syscall		                # printf("\t")

  j INNERLOOP1

ENDOUTERLOOP:
  lw $s2, 0($sp)
  lw $s1, 4($sp)
  lw $s0, 8($sp)
  addi $sp, $sp, 12
#-----------------------------------------------------
# Your code "END" here
#-----------------------------------------------------
jr  $ra##################################################
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
