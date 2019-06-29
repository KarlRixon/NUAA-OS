#ifndef _UTILS_STDIO_H
#define _UTILS_STDIO_H

extern int sprintf(char *buffer, char *format, ...);
extern int printf(char *format, ...);
extern void panic(char *format, ...);
extern int puts(char *string);
extern int putchar(int c);
extern void printf_test();

#endif