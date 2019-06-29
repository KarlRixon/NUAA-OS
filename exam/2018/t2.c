//主进程创建两个子进程，主进程通过管道分别与两个子进程连接
//第一个子进程计算1加到50的和，通过管道送给父进程
//第二个子进程计算50加到100的和，通过管道送给父进程
//父进程读取两个子进程的结果，相加打印，结果为5050

#include<stdio.h>
#include<unistd.h>

int main(){
	int fd1[2], fd2[2];
	int i = 0, sum = 0;
	int r1 = 0, r2 = 0;
	pipe(fd1);
	pipe(fd2);
	int pid1 = -1, pid2 = -1;
	
	pid1 = fork();
	if(pid1 > 0)
		pid2 = fork();
	if(pid1 == 0){
		for(i = 1; i<51; i++)
			sum += i;
		write(fd1[1], &sum, sizeof(int)); // 传递指针
		exit(0);
	}
	if(pid2 == 0){
		for(i = 51; i<101; i++)
			sum += i;
		write(fd2[1], &sum, sizeof(int));
		exit(0);
	}
	wait(NULL);
	wait(NULL);
	read(fd1[0], &r1, sizeof(int));
	read(fd2[0], &r2, sizeof(int));
	printf("%d\n", r1 + r2);
	return 0;
}