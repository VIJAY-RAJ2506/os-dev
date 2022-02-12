#ifndef __IDT_H_
#define _IDT_H_

#include <stdint.h>

/* Interrupt descriptor 
 *
 * 63					48 47  46  45 44  43      40 39					32
 * +--------------------------------------+---+------+---+----------+-------------------------------------+
 * |					  |   |	     | 0 |	    |					  |
 * |		OFFSET (16-31)		  | P |	 DPL |   | Gate Type|		Reserved		  |
 * |					  |   |	     |   |	    |					  |
 * +--------------------------------------+---+------+---+----------+-------------------------------------+
 *
 * 31						   16 15						 0
 * +-------------------------------------------------+----------------------------------------------------+
 * |					 	     |							  |
 * |  			Segment Selector	     |			Offset (15 - 0)			  |
 * |						     |							  |
 * +-------------------------------------------------+----------------------------------------------------+
 *
 *
 */

struct idt_desc
{
	uint16_t offset_1;	/* offset bits 0 .. 15 */
	uint16_t seg_sel;	/* segment selector */
	uint8_t zero;		/* unused, set to zero */
	uint8_t type_attr;	/* gate type, dpl and present bit */
	uint16_t offset_2;	/* offset bits 16 .. 31 */

}__attribute__((packed));

struct idtr
{
	uint16_t limit;		// size of descriptor - 1 
	uint32_t offset;	// offset of the interrupt descriptor
}__attribute__((packed));


void idt_init();

#endif
