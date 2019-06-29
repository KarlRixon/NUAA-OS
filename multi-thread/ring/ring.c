#include<stdio.h>
#include<pthread.h>

#define N 4

int buffer[N];

typedef struct sema_t{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

typedef struct param{
	int order;
}param;

void sema_init(sema_t *sema, int value){
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	while(sema->value <= 0){
		pthread_cond_wait(&sema->cond, &sema->mutex);
	}
	sema->value--;
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	++sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex[N];
sema_t full[N];

void *porter(void *arg){
	int receive;
	param *p = (param *)arg;
	int order = p->order;
	if(order == 0){			// 第一个节点
		sema_wait(&mutex[order+1]);
		buffer[order+1] = 1;
		printf("T1 send 1\n");
		sema_signal(&mutex[order+1]);
		sema_signal(&full[order+1]);
		
		sema_wait(&full[0]);
		sema_wait(&mutex[0]);
		receive = buffer[0];
		printf("T%d received %d\n", 1, receive);
		sema_signal(&mutex[0]);
		exit(0);
	}
	else if(order == N-1){	// 最后一个节点
		sema_wait(&full[order]);
		sema_wait(&mutex[order]);
		receive = buffer[order];
		printf("T%d received %d\n", order+1, receive);
		sema_signal(&mutex[order]);
		
		sema_wait(&mutex[0]);
		buffer[0] = receive+1;
		printf("T%d send %d\n", order+1, receive+1);
		sema_signal(&mutex[0]);
		sema_signal(&full[0]);
	}
	else{					// 中间节点
		sema_wait(&full[order]);
		sema_wait(&mutex[order]);
		receive = buffer[order];
		printf("T%d received %d\n", order+1, receive);
		sema_signal(&mutex[order]);
		
		sema_wait(&mutex[order+1]);
		buffer[order+1] = receive+1;
		printf("T%d send %d\n", order+1, receive+1);
		sema_signal(&mutex[order+1]);
		sema_signal(&full[order+1]);
	}
}

int main(){
	pthread_t porter_tid[N];
	param p[N];
	int i;
	for(i = 0; i<N; i++){
		sema_init(&mutex[i], 1);
		sema_init(&full[i], 0);
	}
	for(i = 0; i<N; i++){
		p[i].order = i;
		pthread_create(&porter_tid[i], NULL, porter, &p[i]);
	}
	for(i = 0; i<N; i++){
		pthread_join(porter_tid[i], NULL);
	}
	return 0;
}