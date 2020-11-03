	.text
main:
	add $t0, $zero, $zero
	add $t1, $zero, $zero
	beq $t0, $t1, B
	add $t2, $t0, $t1
B:	addi $v0,$zero,10
    syscall		# exit()



