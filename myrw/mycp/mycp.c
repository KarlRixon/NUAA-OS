#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char **argv){
	int fd1, fd2;
	int size = 10;
	char buf[size];
	int n;
	fd1 = open(argv[1], O_RDONLY, 0777);
	fd2 = creat(argv[2], 0777);
	while((n = read(fd1, buf, size)) > 0){
		if(write(fd2, buf, n) != n){
			printf("write error\n");
			exit(0);
		}
		if(n < 0){
			printf("read error\n");
		}
	}
	close(fd1);
	close(fd2);
}
