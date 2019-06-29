//主线程创建4个子线程
//四个线程运行代码指定
//使用使用信号量或者条件变量机制使得四个线程满足如下制约关系
//t1的printf执行后，t2和t3才可以执行printf
//t2和t3的printf执行完后，t4才可执行printf
//输出结果为T1 T2 T3 T4或者T1 T3 T2 T4

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define N 4

typedef struct sema_t{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

void sema_init(sema_t *sema, int value){
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	while(sema->value <= 0)
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value--;
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	++sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

sema_t semaT[N];

void *T1_entry(void *arg){
	sleep(2);
	puts("T1");
	sema_signal(&semaT[0]);
	sema_signal(&semaT[0]);
}

void *T2_entry(void *arg){
	sema_wait(&semaT[0]);
	sleep(1);
	puts("T2");
	sema_signal(&semaT[1]);
}

void *T3_entry(void *arg){
	sema_wait(&semaT[0]);
	sleep(1);
	puts("T3");
	sema_signal(&semaT[2]);
}

void *T4_entry(void *arg){
	sema_wait(&semaT[1]);
	sema_wait(&semaT[2]);
	puts("T4");
}

int main(){
	pthread_t t1, t2, t3, t4;
	sema_init(&semaT[0], 0);
	sema_init(&semaT[1], 0);
	sema_init(&semaT[2], 0);
	sema_init(&semaT[3], 0);
	pthread_create(&t1, NULL, T1_entry, NULL);
	pthread_create(&t2, NULL, T2_entry, NULL);
	pthread_create(&t3, NULL, T3_entry, NULL);
	pthread_create(&t4, NULL, T4_entry, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	return 0;
}