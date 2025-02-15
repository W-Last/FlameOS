/* print.h */
#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xb8000
#define WHITE_ON_BLACK 0x0F

void putchar(char c);
void printf(const char *format, ...);

#endif
