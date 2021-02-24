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
jr  $ra