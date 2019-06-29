//父进程创建10个子进程，分别计算1-10,11-20...的累加和
//要求所有进程并行执行，若为串行不得分

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int fds[10][2];
	int i;

    for (i = 0; i < 10; i++) {
        pipe(fds[i]);
		pid_t pid = fork();
        if (pid == 0) {
            int sum = 0;
			int start = i*10+1;
			int end = start+9;
			int ii;
			for(ii=start; ii<=end; ii++)
				sum+=ii;
			write(fds[i][1], &sum, sizeof(int));
			exit(0);
        }
    }

    int sum = 0;
    for (i = 0; i < 10; i++) {
		wait(NULL);
        int r;
		read(fds[i][0], &r, sizeof(int));
        sum += r;
    }
    printf("sum = %d\n", sum);
    return 0;
}
