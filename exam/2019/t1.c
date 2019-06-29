//使用fork,exec,dup,pipe实现命令 cat /etc/passwd | wc -l | cat >log.txt
//父进程创建子进程，子进程执行 cat /etc/passwd 命令，将输出重定向到管道写端
//父进程将输入重定向到管道读端，输出重定向到文件，执行 wc -l 命令

#include <stdio.h>
#include <unistd.h>

int main()
{
	int fd[2];
	char buf[1024];
	int pid;
	pipe(fd);
	pid = fork();
	if(pid == 0){
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execlp("cat", "cat", "/etc/passwd", NULL);
		printf("child process error\n");
	}
	else{
		int fd1 = creat("log.txt", 0777);
		dup2(fd[0], 0);
		dup2(fd1, 1);
		close(fd[0]);
		close(fd[1]);
		wait(NULL);
		execlp("wc", "wc", "-l", NULL);
	}
    return 0;
}
