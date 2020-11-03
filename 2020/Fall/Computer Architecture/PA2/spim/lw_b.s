	.text

main:
	lui	$3, 0x1000
	lw	$2, 0($3)
	bne	$2, $3, B
	add	$3, $0, $0
B:
	and 	$4, $2, $5
	or	$8, $2, $6
	add	$9, $4, $2
	slt 	$3, $6, $7
    addi $v0,$zero,10
    syscall         # exit()



.data 0x10000000
	.word	1, 2, 0 
