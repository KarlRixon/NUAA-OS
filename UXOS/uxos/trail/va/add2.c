#include <stdio.h>

typedef char *va_list;

#define va_start(ap, last_arg) 						                      \
    (ap = ((char *)&(last_arg) + sizeof(last_arg)))

#define va_end(ap)

#define va_arg(ap, type)						                           \
    (ap += sizeof(type),					                               \
    *((type *)(ap - sizeof(type))))

int add(int argc, ...)
{
	int i;
    va_list ap;
    int sum = 0;
    
    va_start(ap, argc);
    for (i = 0; i < argc; i++) {
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