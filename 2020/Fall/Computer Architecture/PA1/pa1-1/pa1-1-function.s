GCD:
#-----------------------------------------------------
# Your code "START" here
#-----------------------------------------------------
  addi $sp, $sp, -12
  sw $s0, 8($sp)
  sw $s1, 4($sp)
  sw $s2, 0($sp)
  slt $s0, $a0, $a1
  bne $s0, $0, L1
  move $s1, $a1
  j L2
L1:
  move $s1, $a0
L2:
  move $s0, $s1
  j L4  
L3:
  addi $s0, $s0, -1
L4:  
  beq $s0, $0, L5
  rem $s2, $a0, $s0
  bne $s2, $0, L3
  rem $s2, $a1, $s0
  bne $s2, $0, L3
  move $v0, $s0
  j L5
L5:
  lw $s2, 0($sp)
  lw $s1, 4($sp)
  lw $s0, 8($sp)
  addi $sp, $sp, 12
#-----------------------------------------------------
# Your code "END" here
#-----------------------------------------------------
jr  $ra

LCM:
#-----------------------------------------------------
# Your code "START" here
#-----------------------------------------------------
  addi $sp, $sp, -12
  sw $s0, 8($sp)
  sw $s1, 4($sp)
  sw $s2, 0($sp)
  sgt $s0, $a0, $a1
  bne $s0, $0, L6
  move $s1, $a1
  j L7
L6:
  move $s1, $a0
L7:
  move $s0, $s1
  j L9
L8:
  add $s0, $s0, $s1
L9:  
  rem $s2, $s0, $a0
  bne $s2, $0, L8
  rem $s2, $s0, $a1
  bne $s2, $0, L8
  move $v0, $s0
  j L10
L10:
  lw $s2, 0($sp)
  lw $s1, 4($sp)
  lw $s0, 8($sp)
  addi $sp, $sp, 12
#-----------------------------------------------------
# Your code "END" here
#-----------------------------------------------------
jr  $ra