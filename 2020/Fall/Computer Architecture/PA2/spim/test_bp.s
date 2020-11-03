.text
main:
	addi $t0, $0, 30	# i = 30
LOOP:
    addi $t0, $t0, -1	# i --
    bne $t0, $0, LOOP	# if ( i != 0 )
	addi $v0,$zero,10
    syscall         	# exit()

