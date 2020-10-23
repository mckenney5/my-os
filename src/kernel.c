#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "include/stdio.h"
#include "include/vga.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
 static void halt_and_catch_fire(){
	 __asm__("cli\n.hang:\nhlt\njmp .hang\n.end:");
 }
 
static void panic(char* msg){
	// unrecoverable error, throw a message then die
	terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
	printf("\nKERNEL: ");
	printf(msg);
	puts(" PANIC!");

halt_and_catch_fire();
}

void sleep(unsigned int scale){
	// poor mans sleep, not accurate AT ALL and I hope you like cooling fans, TODO use clock
	unsigned long i = scale * 221720 * 10;
	for( ; i != 0; i--)
		__asm__("NOP");
}

void kernel_init(){
// do something once
	terminal_initialize();
	printf("Welcome!\n");
	return;
}

void kernel_loop(){
// do this forever
	char i=0;
	while(1){
	// write all chars to the screen
		i++;
		if(i != '\n' && i != '\v') putchar(i);
		sleep(2);
		break;
	}
	return;
}
 
void kernel_main(void){
	kernel_init();
	kernel_loop();
	panic("No More Instructions");
}
