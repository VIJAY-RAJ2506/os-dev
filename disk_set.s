
.code16

.type reset, @function
reset:
	pusha
	
	
	mov $0x00, %ah
	int $0x13

	push %ax
	call status
	pop %ax
	popa 
	ret

.type status, @function
status:
	pusha
	mov %sp, %bp
	
	mov 4(%bp), %ax

	mov %ah, %al
	mov $0x0E, %ah
	int $0x10
	
	popa
	ret
	
.type disk_load, @function
disk_load:
	push %bp
	mov %sp, %bp

	mov $0x02, %ah
	mov $0x01, %al
	mov $0x00, %ch
	mov $0x02, %cl
	mov $0x00, %ah
	mov $0x80, %dl
	xor %bx, %bx
	mov %bx, %es
	int $0x13

	add $'A', %ah	
	mov %ah, %al
	mov $0x0E, %ah
	int $0x10
	
	pop %bp
	ret
	#push %ax
	#call status
	#pop %ax
	#popa


DISK_ERROR:
	.asciz "DISK_ERROR"
