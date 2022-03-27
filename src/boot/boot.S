
.section .text
.code16
.globl _start

_start:

	cli
	ljmp $0, $Begin

Begin:
	mov $0x00, %ax
	mov %ax, %bx
	mov %ax, %cx
	mov %ax, %dx
	mov $0x7c00, %bp
	mov %bp, %sp
	sti


PROTECTED_MODE:

	.equ CODE_SEG, 8
	.equ DATA_SEG, gdt_data - gdt_start

	cli
	lgdt gdt_descriptor
	mov %cr0, %eax
	orl $0x01, %eax
	mov %eax, %cr0

	ljmp $CODE_SEG, $load32


gdt_start:
gdt_null:
	.quad 0x00
gdt_code:
	.word 0xffff
	.word 0x00
	.byte 0x00
	.byte 0b10011010
	.byte 0b11001111
	.byte 0x00
gdt_data:
	.word 0xffff
	.word 0x00
	.byte 0x00
	.byte 0b10010010
	.byte 0b11001111
	.byte 0x00
gdt_end:
gdt_descriptor:
	.word gdt_end - gdt_start
	.long gdt_start

.code32
load32:
	mov $0x01, %eax
	mov $0x64, %ecx
	mov $0x100000, %edi

ata_read:
	mov %eax, %ebx
	shr $24, %eax
	or $0xE0, %eax
	mov $0x1F6, %dx
	out %al, %dx


	mov %ecx, %eax
	mov $0x1F2, %edx
	out %al, %dx

	mov %ebx, %eax
	mov $0x1F3, %dx
	out %al, %dx

	mov %ebx, %eax
	mov $0x1F4, %dx
	shr $0x08, %eax
	out %al, %dx

	mov %ebx, %eax
	mov $0x1F5, %dx
	shr $0x10, %eax
	out %al, %dx

	mov $0x1F7, %dx
	mov $0x20, %al
	out %al, %dx


next_sector:
	push %ecx

try_again:
	mov $0x1F7, %dx
	in %dx, %ax
	test $0x08, %ax
	jz try_again

	mov $256, %ax
	mov $0x1F0, %dx

	rep insw

	pop %ecx
	loop next_sector

	ljmp $CODE_SEG, $0x100000

	.fill 510-(.-_start),1,0

	. = _start + 510
	.byte 0x55
	.byte 0xaa

