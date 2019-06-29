#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
int main(int argc, char **argv){
	int size = 4096;
	char buf[size];
	int fd;
	int index;
	printf("%s\n", argv[0]);
	fd = open(argv[1], O_RDONLY, 0777);
	while(index = read (fd, buf, size) > 0){
		printf("%s", buf);
		if(index < 0)
			printf("read error\n");
	}
	close(fd);
}
