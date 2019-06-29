#include<utils/stdio.h>
int main() 
{
    char *vga_low = (char *)0xB8000;
    vga_low[0] = 'X';
    vga_low[1] = 0x0C;

    char *vga_high = (char *)0xC00B8000;
    vga_high[2] = 'Y';
    vga_high[3] = 0x0C;

	vga_init();
    vga_puts("Hello\n");
    vga_puts("World\n");
	printf("%s\n", "Welcome");
	printf("%s\n", "This is UXOS");
	printf_test();
    while (1);
    return 0;
}