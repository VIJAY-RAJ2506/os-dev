
.section .text
.global idt_load
.type idt_load, @function
idt_load:
	pushl %ebp
	movl %esp, %ebp
	
	mov 8(%ebp), %ebx
	
	lidt (%ebx)
	
	movl %ebp, %esp
	popl %ebp
	ret

	
