#ifndef _USING_VGA_H
#define _USING_VGA_H

#include <stddef.h>
#include <stdint.h>

#define TAB_SIZE 2
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
 
/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}


static uint16_t screen_buff[VGA_WIDTH * VGA_HEIGHT] = {0};
 
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void clear_screen(){
// writes spaces onto the whole screen
	
	for (size_t y = 0; y < VGA_HEIGHT; y++){
		for (size_t x = 0; x < VGA_WIDTH; x++){
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	// moves the printing head to 0,0
	terminal_row = 0;
	terminal_column = 0;
}

void copy_screen(){
// copies what is on the screen to memory for future use
	size_t i = 0;
	for(i=0; i != VGA_WIDTH * VGA_HEIGHT; i++) // clear old data
		screen_buff[i] = 0;

	for(i=0; i != VGA_WIDTH * VGA_HEIGHT; i++) // copy
		screen_buff[i] = terminal_buffer[i];
	
}

static void scroll(){
// scrolls the screen up one line
	
	copy_screen();
	clear_screen();

	// rewrite the screen but skip the first line
	size_t i = 0, l=0;
	for(i=VGA_WIDTH; i != VGA_WIDTH * VGA_HEIGHT; i++)
		terminal_buffer[l++] = screen_buff[i];
	
	// move the printing head to the last line
	terminal_row = VGA_HEIGHT-1;
}

void terminal_initialize(void){
// sets up the screen colors and sets up the screen buffer
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	clear_screen();
}
 
void terminal_set_color(enum vga_color fg, enum vga_color bg){
	terminal_color = vga_entry_color(fg, bg);
}
 
static void terminal_put_entry_at(int c, uint8_t color, size_t x, size_t y){
// writes to the screen, character - text-color - location
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static void inc_row(){
// increases the row and checks if we hit the end of the screen
	if(++terminal_row >= VGA_HEIGHT)
		scroll();
}

static void inc_column(){
// increases the column and checks if we hit the end of the screen
	if (++terminal_column == VGA_WIDTH) { //if we wrote all the way right
		terminal_column = 0; // move the printing head left
		inc_row();
	}
}

 
void terminal_putchar(int c){
// external function to display text
	switch((char) c){
		case '\n':
			terminal_column = 0;
			inc_row();
			break;
		case '\t':
			terminal_column += TAB_SIZE;
			inc_column();
			break;
		case '\v':
			terminal_row += TAB_SIZE-1;
			inc_row();
			break;
		case '\b':
			if(terminal_column != 0) terminal_column--;
			break;
		case '\a':
			// TODO Bell
			break;
		case '\0':
			break;
		case '\r':
			break;
		case -1: //EOF
			break;
		default:
			terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);
			inc_column();
	}
}

#endif
