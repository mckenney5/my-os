#ifndef _USING_STDIO_H
#define _USING_STDIO_H

#include "vga.h"

const int EOF = -1;
//const int NULL = 0;

enum file_descriptor {
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2,
};

/*
enum stream_orentation {
	UNSET = 0,
	NARROW = 1,
	WIDE = 2,
};

enum io_mode {
	UNBUFFERED = 0,
	LINE_BUFFERED = 1,
	FULLY_BUFFERED = 2,
};

struct File {
	/ *
	-platform-specific identifier of the associated I/O device, such as a file descriptor
	-the buffer
	-stream orientation indicator (unset, narrow, or wide)
	-stream buffering state indicator (unbuffered, line buffered, fully buffered)
	-I/O mode indicator (input stream, output stream, or update stream)
	-binary/text mode indicator
	-end-of-file indicator
	-error indicator
	-the current stream position and multibyte conversion state (an object of type mbstate_t)
	 * /
	unsigned int fd;
	int *buf;
	
} FILE;

int fputc(int c, FILE *stream){
 // not implemented
}

int fputs(const char *s, FILE *stream){
 // not implemented
}

int putc(int c, FILE *stream){
 // not implemented
}
*/
	
int putchar(int c){
 // returns the character written as an unsigned char cast to an int or EOF on error.
	terminal_putchar(c);
	return c;
}

int puts(const char *s){
	// returns a nonnegative number on success, or EOF on error.
	unsigned int i=0;
	for(i=0; s[i] != '\0'; i++){
		if(putchar((int)s[i]) == EOF) return EOF;
	}
	if(putchar((int)'\n') == EOF) // trailing new line
		return EOF;
	else
		return i;
}

int printf(const char *format, ...){
	// dummy function TODO add formatting
	unsigned int i=0;
	for(i=0; format[i] != '\0'; i++){
		if(putchar((int)format[i]) == EOF) return EOF;
	}
	return i-1;
}
#endif
