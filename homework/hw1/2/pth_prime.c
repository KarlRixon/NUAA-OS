#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define WORKERS 15
#define SIZE 2000000		// 如果运行出错可能是SIZE过小

typedef struct param{
	int i;
	int average;
	void *shm1;
	void *shm2;
}param;

typedef struct shary{
	int prime[SIZE];
}shary;

int prime(int num){
	int i = 0;
	int k = (int)sqrt(num)+1;
	for(i = 2; i<k; i++){
		if(num%i == 0)
			return 0;
	}
	return 1;
}

void *compute(void *arg){
	param *p = (param *)arg;
	int i = p->i;
	int average = p->average;
	shary *shm1 = (shary *)p->shm1;
	shary *shm2 = (shary *)p->shm2;
	
	//FILE *fd = fopen("out.txt", "w");
	//fprintf(fd, "%d\n", p->i);
	
	int j = 0;
	int count = 0;
	for(j = i*average; j<(i+1)*average; j++){
		if(prime(j+1)){
			shm1->prime[count++] = (j+1);
		}
	}
	shm1->prime[count] = -1;
	count = 0;
	for(j = (i+WORKERS)*average; j<(i+1+WORKERS)*average; j++){
		if(prime(j+1)){
			shm2->prime[count++] = (j+1);
		}
	}
	shm2->prime[count] = -1;
}

int main(int argc, char **argv){
	// 处理输入
	if(argc<2){
		fprintf(stderr, "no number input\n");
		exit(0);
	}
	int n = atoi(argv[1]);
	int average;
	if(n%(WORKERS*2) != 0)
		average = n/(WORKERS*2)+1;		// 可能不能整除所以加1
	else average = n/(WORKERS*2);
	
	// 获取共享空间
	int i = 0;
	void *shm[WORKERS*2];		// 指针数组
	shary *shared;				// 共享内存指针
	int shmid[WORKERS*2];		// 共享内存id
	for(i = 0; i<WORKERS*2; i++){
		shmid[i] = shmget(IPC_PRIVATE, sizeof(shary), IPC_CREAT|IPC_EXCL|0600);	//需要用0600指定权限
		if(shmid[i] == -1){
			fprintf(stderr, "shmget failed\n");
			exit(0);
		}
	}
	for(i = 0; i<WORKERS*2; i++){
		shm[i] = shmat(shmid[i], NULL, 0);
		if(shm[i] == (void *)-1){
			fprintf(stderr, "shmat failed\n");
			exit(0);
		}
	}
	
	// 创建多线程
	pthread_t worker_tids[WORKERS];		// tid输出要以long int形式
	param args[WORKERS];
	param *arg = NULL;
	
	long long int a = clock();
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for(i = 0; i<WORKERS; i++){
		arg = &args[i];
		arg->i = i;
		arg->average = average;
		arg->shm1 = shm[i*2];
		arg->shm2 = shm[i*2+1];
		int r = pthread_create(&worker_tids[i], NULL, &compute, arg);
		if(r != 0){
			fprintf(stderr, "create thread failed");
			exit(0);
		}
	}
	
	// 等待所有线程执行完毕
	for(i = 0; i<WORKERS; i++){
		pthread_join(worker_tids[i], NULL);
	}
	gettimeofday(&end, NULL);
	printf("total time: %ldms\t%lds\n", end.tv_usec-start.tv_usec, end.tv_sec-start.tv_sec);
	long long int b = clock();
	
	// 升序输出结果
	int j = 0;
	for(i = 0; i<WORKERS; i++){
		for(j = 0; j<SIZE; j++){
			if(((shary *)shm[i*2])->prime[j] == -1){
				break;
			}
			printf("%d\t", ((shary *)shm[i*2])->prime[j]);
		}
	}
	for(i = 0; i<WORKERS; i++){
		for(j = 0; j<SIZE; j++){
			if(((shary *)shm[i*2+1])->prime[j] == -1){
				break;
			}
			printf("%d\t", ((shary *)shm[i*2+1])->prime[j]);
		}
	}
	puts("");
	printf("total time: %ldms\t%lds\n", end.tv_usec-start.tv_usec, end.tv_sec-start.tv_sec);
	printf("total time: %lldms\n", b-a);
	
	return 0;
}