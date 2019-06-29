#include <stdio.h>
#include <unistd.h>

int global = 0;

void child()
{
    for (int i = 0; i < 3; i++) {
        global++;
        printf("In child, global = %d\n", global);
        sleep(1);
    }
}

void parent()
{
    for (int i = 0; i < 3; i++) {
        global++;
        printf("In parent, global = %d\n", global);
        sleep(1);
    }
} 

int main()
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
        child();
    else
        parent();
    return 0;
}
