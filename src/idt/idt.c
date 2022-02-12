#include <idt/idt.h>
#include <memory/memory.h>
#include <kernel.h>
#include <config.h>

struct idt_desc idt_descriptor[MAX_INTERRUPT];
struct idtr idtr_descriptor;

extern void idt_load(void *ptr);

void idt_zero()
{
	print("Divide by zero error \n");
}

void idt_set(unsigned int intr_no, void *address) {

	struct idt_desc *desc = &idt_descriptor[intr_no];
	desc->offset_1 = (uint32_t)address & 0x0000FFFF;
	desc->seg_sel = KERNEL_CODE_SEL;
	desc->zero = 0x00;
	desc->type_attr = 0xEE;		/*
					 * E(lower bits) :- For 32 bit interrupt gate
					 * 0 (4th bit) :- always set to zero
					 * 0b11 (5th and 6th bit) :- Descriptor privlege level (value is for user level interrupt)
					 * 1 (7th bit) :- Present bit. set it to '1' if the interrupt in active
					 */
	desc->offset_2 = (uint32_t)address >> 16;

}

void idt_init() {

	memset(idt_descriptor, 0, sizeof(idt_descriptor));
	idtr_descriptor.limit = sizeof(idt_descriptor) - 1;
	idtr_descriptor.offset = (uint32_t)idt_descriptor;

	idt_set(0,idt_zero);

	idt_load(&idtr_descriptor);
}

