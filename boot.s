

.code16
.section .text
.global _start
_start:

	mov $0x8000, %bp
	mov %bp, %sp
	

read:
	mov $0x9000, %bx	
	mov $0x02, %ah
	mov $0x01, %al
	mov $0x00, %ch
	mov $0x02, %cl
	mov $0x00, %ah
	mov $0x80, %dl
	mov $0x00, %dh
        int $0x13
	
	mov $'A', %al
	mov $0x0e, %ah
	int $0x10

end:
	. = _start + 510
	.word 0xaa55

	
	
