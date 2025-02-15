/* print.c */
#include "print.h"
#include <stdarg.h>

// Global cursor position for VGA text mode
static int cursor_row = 0;
static int cursor_col = 0;

void putchar(char c) {
    volatile unsigned short *video = (volatile unsigned short *)VGA_ADDRESS;
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else {
        video[cursor_row * VGA_WIDTH + cursor_col] = (unsigned short)c | (WHITE_ON_BLACK << 8);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    // Wrap around if necessary (or implement scrolling)
    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0;
    }
}

// Helper: Reverse a string in place.
static void reverse(char *str, int length) {
    int start = 0, end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Helper: Convert integer to string in the given base.
static void itoa(int value, char *buffer, int base) {
    int i = 0;
    int isNegative = 0;
    
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    
    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }
    
    while (value != 0) {
        int rem = value % base;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value /= base;
    }
    
    if (isNegative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0';
    reverse(buffer, i);
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    while (*format) {
        if (*format == '%') {
            format++;
            if (!*format) break;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    while (*str) {
                        putchar(*str++);
                    }
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buf[32];
                    itoa(num, buf, 10);
                    char *p = buf;
                    while (*p)
                        putchar(*p++);
                    break;
                }
                case 'x': {
                    int num = va_arg(args, int);
                    char buf[32];
                    itoa(num, buf, 16);
                    char *p = buf;
                    while (*p)
                        putchar(*p++);
                    break;
                }
                case 'c': {
                    char ch = (char)va_arg(args, int);
                    putchar(ch);
                    break;
                }
                case '%': {
                    putchar('%');
                    break;
                }
                default:
                    // Unrecognized specifier, print it literally.
                    putchar('%');
                    putchar(*format);
                    break;
            }
        } else {
            putchar(*format);
        }
        format++;
    }
    
    va_end(args);
}
