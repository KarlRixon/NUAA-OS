#include <utils/root.h>
#include "vga.h"

#define vga_row_number 25
#define vga_column_number 80
vga_char_t *vga_base;
vga_char_t *vga_bound;
vga_char_t *vga_cursor;

void vga_char_set(vga_char_t *this, char value)
{
    this->value = value;
    this->attr = VGA_NORMAL;
}

void vga_init()
{
    vga_char_t *vga_char;

    vga_base = (vga_char_t *) 0xC00B8000;
    vga_bound = vga_base + vga_row_number * vga_column_number;

    for (vga_char = vga_base; vga_char < vga_bound; vga_char++)
        vga_char_set(vga_char, ' ');

    vga_cursor = vga_base;
}

void vga_put_char(char c)
{
    int row, column;

    row = (vga_cursor - vga_base) / vga_column_number;
    column = (vga_cursor - vga_base) % vga_column_number;
    switch (c) {
        case '\n':
			//源码有误，作以下修改
            //row++;
            //column = 0;
			vga_cursor += (80-column);
            break;

        default:
            vga_char_set(vga_cursor, c);
            vga_cursor++;
    }
}

void vga_write(char *buffer, int size)
{
    int i;

    for (i = 0; i < size; i++)
        vga_put_char(buffer[i]);
}

void vga_puts(char *string)
{
    char c;

    for (; c = *string; string++)
        vga_put_char(c);
    vga_put_char('\n');
}