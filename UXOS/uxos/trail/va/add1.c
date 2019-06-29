#include <stdio.h>
#include <stdarg.h>

int add(int argc, ...)
{
    va_list ap;
    int sum = 0;
    
    va_start(ap, argc);
    for (int i = 0; i < argc; i++) {
        sum += va_arg(ap, int);
    }
    va_end(ap);

    return sum;
}

int main()
{
    printf("add(1, 11) = %d\n", add(1, 11));
    printf("add(2, 11, 22) = %d\n", add(2, 11, 22));
    printf("add(3, 11, 22, 33) = %d\n", add(3, 11, 22, 33));
    return 0;
}