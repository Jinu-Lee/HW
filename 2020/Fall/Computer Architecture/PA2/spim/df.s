	.text
main:
	add $t0, $zero, $zero
	addi $t1, $t0, 3
	sub $t2, $t1, $t0
    addi $v0,$zero,10
    syscall         # exit()
	
