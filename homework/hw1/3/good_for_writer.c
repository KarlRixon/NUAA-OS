#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

// semaphores
int writeCount, readCount;
sem_t RWmutex;		// 读者写者互斥
sem_t WWmutex;		// 写者写者互斥
sem_t RCmutex;		// 防止多个读者修改writeCount
sem_t WCmutex;		// 防止多个写者修改readCount
sem_t Wmutex;		// 防止写者与多个读者竞争

typedef struct data{
	int id;
	int arvTime;
	int lastTime;
	char role;
}data;

void *reader(void *param){
	int id = ((data *)param)->id;
	int lastTime = ((data *)param)->lastTime;
	int arvTime = ((data *)param)->arvTime;
	
	sem_wait(&Wmutex);
	sem_wait(&RWmutex);
	sem_wait(&RCmutex);
	readCount++;
	if(readCount == 1){
		sem_wait(&WWmutex);
	}
	sem_post(&RCmutex);
	sem_post(&RWmutex);
	sem_post(&Wmutex);
	
	printf("reader %d is reading\n", id);
	sleep(lastTime);
	printf("reader %d is leaving\n", id);
	
	sem_wait(&RCmutex);
	readCount--;
	if(readCount == 0){
		sem_post(&WWmutex);
	}
	sem_post(&RCmutex);
	
	pthread_exit(0);
}

void *writer(void *param){
	int id = ((data *)param)->id;
	int lastTime = ((data *)param)->lastTime;
	int arvTime = ((data *)param)->arvTime;
	
	sem_wait(&WCmutex);
	writeCount++;
	if(writeCount == 1){
		sem_wait(&RWmutex);
	}
	sem_post(&WCmutex);
	
	sem_wait(&WWmutex);
	printf("writer %d is writing\n", id);
	sleep(lastTime);
	printf("writer %d is leaving\n", id);
	sem_post(&WWmutex);
	
	sem_wait(&WCmutex);
	writeCount--;
	if(writeCount == 0){
		sem_post(&RWmutex);
	}
	sem_post(&WCmutex);
	
	pthread_exit(0);
}

int main(){
	int n = 0;
	scanf("%d", &n);
	pthread_t tid[n];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	int id;
	char role;
	int arvTime;
	int lastTime;
	data *d[n];
	
	sem_init(&RWmutex, 0, 1);
	sem_init(&WWmutex, 0, 1);
	sem_init(&RCmutex, 0, 1);
	sem_init(&WCmutex, 0, 1);
	sem_init(&Wmutex, 0, 1);
	readCount = writeCount = 0;
	
	// 读取输入 按到达顺序输入
	int i = 0;
	for(i = 0; i<n; i++){
		scanf("%d %c %d %d\n", &id, &role, &arvTime, &lastTime);
		d[i] = (data *)malloc(sizeof(data));
		d[i]->id = id;
		d[i]->role = role;
		d[i]->arvTime = arvTime;
		d[i]->lastTime = lastTime;	
	}
	
	// 创建线程
	for(i = 0; i<n; i++){
		if(i == 0){
			sleep(d[0]->arvTime);
		}
		else{		// 输入必须按照达到时间顺序
			sleep(d[i]->arvTime - d[i-1]->arvTime);
		}
		if(d[i]->role == 'R'){
			pthread_create(&tid[i], &attr, reader, d[i]);
		}
		else if(d[i]->role == 'W'){
			pthread_create(&tid[i], &attr, writer, d[i]);
		}
	}
	
	for(i = 0; i<n; i++){
		pthread_join(tid[i], NULL);
	}
	
	sem_destroy(&RWmutex);
	sem_destroy(&WWmutex);
	sem_destroy(&RCmutex);
	sem_destroy(&WCmutex);
	sem_destroy(&Wmutex);
	
	return 0;
}