
.section .text
.equ CODE_SEG, 0x08
.equ DATA_SEG, 0x10
.global _start
.extern kernel_init
.type _start, @function
_start:
	mov $DATA_SEG, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $0x00200000, %ebp
        mov %ebp, %esp

//Enabling A20 line 

	in $0x92, %al
	or $0x02, %al
	out %al, $0x92
	call kernel_init

loop:
	hlt
	jmp loop	
