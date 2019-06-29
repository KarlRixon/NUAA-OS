#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

#define SIZE 1024

typedef struct shary{
	long long int fib[SIZE];
}shary;

int main(int argc, char **argv){
	// 输入参数
	if(argc < 2){
		fprintf(stderr, "no argument input\n");
		exit(0);
	}
	int n = atoi(argv[1]);
	int shmcnt = 0;
	if(n <= 0){
		fprintf(stderr, "input n error\n");
		exit(0);
	}
	shmcnt = (n/SIZE + 1);
	//printf("%d\n", shmcnt);
	
	// 创建并连接共享内存
	void *shm[shmcnt];		// 指向多块共享共享内存的指针数组，由于斐波那契数列增长迅速，导致保存结果用不了多少内存空间，所以申请多块内存的想法在这题中没有实用意义
	shary *shared;			// 用结构体指针指向一块共享内存来进行读写操作
	int shmid[shmcnt];		// 共享内存id数组
	
	int i=0, j=0;
	for(i=0; i<shmcnt; i++){
		shmid[i] = shmget(IPC_PRIVATE, sizeof(shary), IPC_CREAT|IPC_EXCL|0600);	//需要用0600指定权限
		if(shmid[i] == -1){
			fprintf(stderr, "shmget failed\n");
			exit(0);
		}
	}
	for(i = 0; i<shmcnt; i++){
		shm[i] = shmat(shmid[i], NULL, 0);
		if(shm[i] == (void *)-1){
			fprintf(stderr, "shmat failed\n");
			exit(0);
		}
	}
	
	// 创建子进程
	int count = 0;
	int pid = fork();
	if(pid == 0){
		long long int a = 0, b = 1;
		shared = (shary *)shm[0];
		shared->fib[0] = a;
		shared->fib[1] = b;
		count = 2;
		for(i = 0; i<shmcnt; i++){
			for(j = 0; j<1024; j++){
				if(count == 2 && j < 2){
					j = 1;
					continue;
				}
				shared->fib[j] = (a+b);
				a = b;
				b = shared->fib[j];
				count++;
				if(count >= n)
					break;
			}
			if(i<(shmcnt-1))
				shared = (shary *)shm[i+1];
		}
		exit(0);
	}
	else
		wait(NULL);
	
	// 父进程读取共享内存中的计算结果
	count = 0;
	for(i = 0; i<shmcnt; i++){
		shared = (shary *)shm[i];
		for(j = 0; j<1024; j++){
			printf("%lld\t", shared->fib[j]);
			count++;
			if(count >= n)
				break;
		}
	}
	puts("");
}
