
.macro PUSH_ADX
	push %ax
	push %bx
	push %cx
	push %dx
.endm

.macro POP_DAX
	pop %dx
	pop %cx
	pop %bx
	pop %ax
.endm

.macro HEX_NIBBLE reg
	LOCAL letter,end
	cmp $10,\reg
	jae letter
	add $0, \reg
	jmp end

letter:
	add $0x37,\reg
end:
.endm

.macro BEGIN
	.code16
	cli
	ljmp $0, $1f
1:
	xor %ax, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %bp
	mov %ax, %ss
	mov %bp, %sp

.endm

.macro CURSOR_POS x=$0, y=$0
	PUSH_ADX
	mov $0x02, %ah
	mov $0x00, %bh
	mov \x, %dh
	mov \y, %dl
	int $0x10
	POP_DAX
.endm

.macro CLEAR
	PUSH_ADX
	mov $0x0600, %ax
	mov $0x0F, %bh			//Background 0=black, Text F=white
	mov $0x0000, %cx
	mov $0x184f, %dx
	int $0x10
	CURSOR_POS
	POP_DAX
.endm

.macro PUT_CHAR c
	mov \c, %al
	mov $0x0e, %ah
	int $0x10
.endm

.macro STAGE2
	mov $__stage2_nsectors, %al	//defined in linker script
	mov $0x02, %ah
	mov $0x0002, %cx
	mov $1f, %bx
	mov $0x00, %dh
	mov $0x00, %dl
	int $0x13
	jmp 1f
1:
.section .stage2

.endm

.macro PROTECTED_MODE
	.equ CODE_SEG, 8
	.equ DATA_SEG, gdt_data - gdt_start

	cli
	lgdt gdt_descriptor
	
	/* setting bit0 of cr(control register)0 register to enable protected mode */

	mov %cr0, %eax
	orl $0x01, %eax
	mov %eax, %cr0

	ljmp $CODE_SEG, $protected_mode

gdt_start:
gdt_null:
	/* 
	 * The first segment is always set to 0 and is called the Null Segment. 
	 * This is reserved by Intel. If we try to load the Null Segment, a General 
	 * Protection Exception will occur. We specify the Null Segment, by writing 
	 * a 64-bits containing 0
	 * As for the reason, refer below link
	 *
	 * http://stackoverflow.com/questions/33198282/why-have-the-first-segment-descriptor-of-the-global-descriptor-table-contain-onl
	 */
	.quad 0x00

/*
 * 	31			24  23  22  21  19	16  15    13  11  (Type) 8 7			     0
 *	+-----------------------+---+---+---+---+-------+---+-----+---+--+--+--+--+--------------------------+
 *	|		   	|   | D |   | A |  SEG	|   |  D  |   |	E| D| R| A|		 	     |	
 *	|	Base 31:24	| G | / | L | V | LIMIT	| P |  P  | S |	X| C| W| C|	Base 23:16	     | 4
 *	|		   	|   | B |   | L | 19:16	|   |  L  |   |	 |  |  |  |			     |
 *	+-----------------------+---+---+---+---+-------+---+-----+---+-----------+--------------------------+
 *
 *	31						15						     0
 *	+-------------------------------------------------+--------------------------------------------------+
 *	|						  |						     |
 *	|		Base Address 15:00		  |		Segment Limit 15:00		     | 0
 *	|						  |						     |
 *	+-------------------------------------------------+--------------------------------------------------+
 * 
 *	Segment Limit
 *	Base Address 15:00 
 *	Base Address 16:23
 *
 *	Type (8th bit - 11th bit)
 *		Type:
				bit order			Description
				   8			Ac: Accessed bit. Just set to 0. 
				   			    The CPU sets this to 1 when 
							    the segment is accessed.

				   9 			RW: Readable bit/Writable bit.
				   			    - Readable bit for code selectors: 
							      Whether read access for this segment is allowed. 
							      Write access is never allowed for code segments.
							    - Writable bit for data selectors: 
							      Whether write access for this segment is allowed. 
							      Read access is always allowed for data segments.

	  			   10			DC:  Direction bit/Conforming bit.
				  			     - Direction bit for data selectors: 
							       Tells the direction. 0 the segment grows up. 
							       1 the segment grows down, ie. the offset has 
							       to be greater than the limit.
							     - Conforming bit for code selectors:
								- If 1 code in this segment can be executed from an
							          equal or lower privilege level. For example, code in ring 3 											can far-jump to conforming code in a ring 2 segment. 
								  The privl-bits represent the highest privilege level that 
								  is allowed to execute the segment. For example, code in 
								  ring 0 cannot far-jump to a conforming code segment with 
								  privl==0x2, while code in ring 2 and 3 can. Note that 
								  the privilege level remains the same, ie. a far-jump form 
								  ring 3 to a privl==2-segment remains in ring 3 after 
								  the jump. 
								- If 0 code in this segment can only be executed from the 
								  ring set in privl.
				  11			 Ex:  Executable bit. If 1 code in this segment can be executed, 
				  			      ie. a code selector. If 0 it is a data selector.
					   						   

 *	S : Descriptor type. 
 *	    This bit should be set for code or data segments 
 *	    and should be cleared for sys 
 *
 *	DPL (descriptor Privilege level):
 *	Privilege, 2 bits. Contains the ring level, 
 *	0 = highest (kernel), 
 *	3 = lowest (user applications).
 *	
 *	P : Present bit. This must be 1 for all valid selectors.
 *
 *	AVL : Available for use by system software (default value = 0)
 *
 *	L : 64 bit code segment: 0, unused on 32- bit processor
 *
 *	D/B : 	Size bit. If 0 the selector defines 16 bit protected mode. 
		If 1 it defines 32 bit protected mode. You can have both 16 bit
		 and 32 bit selectors at once. 
 *
 *	G : 	Granularity bit. If 0 the limit is in 1 B blocks (byte granularity), 
 *		if 1 the limit is in 4 KiB blocks (page granularity).
 */	   
gdt_code:
	.word 0xFFFF			
	.word 0x00			
	.byte 0x00			
	.byte 0b10011010		
	.byte 0b11001111
	.byte 0x00	
gdt_data:
	.word 0xFFFF
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
protected_mode:
	mov $DATA_SEG, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $0x7000, %esp
	mov %esp, %ebp	  
.endm

