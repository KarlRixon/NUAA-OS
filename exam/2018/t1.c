//主进程创建一个子进程
//主进程通过管道与子进程连接
//子进程标准输出连接到管道写端
//主进程标准输入连接到管道读端
//子进程调用exec("echo", "echo", "hello world", NULL)
//父进程调用read(0, buf, sizeof(buf))并打印

#include<stdio.h>
#include<unistd.h>

int main(){
	int fd[2];
	char buf[32];
	
	pipe(fd);
	int pid = fork();
	if(pid == 0){
		//child
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		
		execlp("echo", "echo", "hello world", NULL);
		printf("child process exec failed\n");	
	}
	else{
		//parent
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		
		wait(NULL);
		read(0, buf, sizeof(buf));
		printf("%s\n", buf);
		return 0;
	}
}