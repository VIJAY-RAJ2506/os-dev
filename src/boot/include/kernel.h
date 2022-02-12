#ifndef KERNEL_H
#define KERNEL_H


#define VGA_WIDTH 80
#define VGA_HEIGHT 20

enum color_code {
	VGA_COLOR_BLACK=0,
	VGA_COLOR_BLUE,
	VGA_COLOR_GREEN,
	VGA_COLOR_CYAN,
	VGA_COLOR_RED,
	VGA_COLOR_PURPLE,
	VGA_COLOR_BROWN,
	VGA_COLOR_GRAY,
	VGA_COLOR_DARK_GREY,
	VGA_COLOR_LIGHT_BLUE,
	VGA_COLOR_LIGHT_GREEN,
	VGA_COLOR_LIGHT_CYAN,
	VGA_COLOR_LIGHT_RED,
	VGA_COLOR_LIGHT_PURPLE,
	VGA_COLOR_YELLOW,
	VGA_COLOR_WHITE
};

void kernel_init();
void terminal_putchar(int x, int y, unsigned char c, enum color_code color);
void print(const char *str);
#endif
