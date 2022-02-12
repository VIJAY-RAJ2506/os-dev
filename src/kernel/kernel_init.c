#include <kernel.h>
#include <stdint.h>
#include <stddef.h>
#include <idt/idt.h>

#define VGA_MEM 0xb8000 

uint16_t vga_width=0;
uint16_t vga_height=0;
uint16_t *vga_mem=0;


uint16_t terminal_make_char(unsigned char ch, enum color_code color)
{	
	return (color<<8)|ch;
}

void terminal_putchar(int x, int y, unsigned char c, enum color_code color)
{
	vga_mem[(y*VGA_WIDTH) + x] = terminal_make_char(c,color);
}

void clear_screen()
{
	vga_mem=(uint16_t*)VGA_MEM;

	for(int i=0; i<VGA_HEIGHT; i++) {
		for(int j=0; j<VGA_WIDTH; j++)
			terminal_putchar(j,i,' ',VGA_COLOR_BLACK);
	}
}

void terminal_write(unsigned char c, enum color_code color)
{
	if(c == '\n') {
		vga_height++;
		vga_width=0;
		return;
	}
	terminal_putchar(vga_width, vga_height, c, color);
	if(vga_width > VGA_WIDTH) {
		vga_height++;
		vga_width=0;
		return;
	}
	vga_width++;
	return;
}

size_t strlen(const char *str)
{
	
	size_t len=0;
	while(str[len])
	{
		len++;
	}

	return len;
}

void print(const char *str)
{
	size_t len = strlen(str);
	for (int i = 0; i < len; i++)
		terminal_write(str[i],VGA_COLOR_WHITE);
}

void kernel_init() {
	clear_screen();
	print("Hello");
}
