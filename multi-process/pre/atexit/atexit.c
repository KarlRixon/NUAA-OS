#include <stdio.h>
#include <stdlib.h>

void f1()
{
    puts("f1");
}
 
void f2()
{
    puts("f2");
}
 
void f3()
{
    puts("f3");
}

int main()
{ 
    atexit(f1);
    atexit(f2);
    atexit(f3);
    puts("main");
    return 0;
}
