#include <stdio.h>

int add(int argc, ...)
{
    int sum = 0;
    int *ap = &argc + 1;

    for (int i = 0; i < argc; i++) {
        int arg = *(ap += 1, ap - 1);
        sum += arg;
    }

    return sum;
}

int main()
{
    printf("add(1, 11) = %d\n", add(1, 11));
    printf("add(2, 11, 22) = %d\n", add(2, 11, 22));
    printf("add(3, 11, 22, 33) = %d\n", add(3, 11, 22, 33));
    return 0;
}