#include<stdio.h>
#include<pthread.h>

#define CAPACITY 4
int buffer1[CAPACITY], buffer2[CAPACITY];
int in1, in2;
int out1, out2;
int buffer1_is_empty(){
	return in1 == out1;
}

int buffer2_is_empty(){
	return in2 == out2;
}

int buffer1_is_full(){
	return (in1 + 1)%CAPACITY == out1;
}

int buffer2_is_full(){
	return (in2 + 1)%CAPACITY == out2;
}

void buffer1_put(int item){
	buffer1[in1] = item;
	in1 = (in1 + 1)%CAPACITY;
}

void buffer2_put(int item){
	buffer2[in2] = item;
	in2 = (in2 + 1)%CAPACITY;
}

int buffer1_get(){
	int item;
	item = buffer1[out1];
	out1 = (out1 + 1)%CAPACITY;
	return item;
}

int buffer2_get(){
	int item;
	item = buffer2[out2];
	out2 = (out2 + 1)%CAPACITY;
	return item;
}

void print_buffer(){
	int i;
	for(i = 0; i<CAPACITY; i++){
		printf("%c ", buffer1[i]);
	}
	puts("\n1");
	for(i = 0; i<CAPACITY; i++){
		printf("%c ", buffer2[i]);
	}
	puts("\n2");
}

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

sema_t mutex1, mutex2;
sema_t empty1, empty2;
sema_t full1, full2;

#define ITEM_COUNT 8

void *producer(){
	int i;
	for(i = 0; i<ITEM_COUNT; i++){
		sema_wait(&empty1);
		sema_wait(&mutex1);
		buffer1_put('a'+i);
		printf("produce %c\n", 'a'+i);
		sema_signal(&mutex1);
		sema_signal(&full1);
	}
}

void *computer(){
	int i, item;
	for(i = 0; i<ITEM_COUNT; i++){
		sema_wait(&full1);
		sema_wait(&mutex1);
		item = buffer1_get();
		sema_signal(&mutex1);
		sema_signal(&empty1);
		
		item += 'A'-'a';
		
		sema_wait(&empty2);
		sema_wait(&mutex2);
		buffer2_put(item);
		printf("compute %c\n", item);
		sema_signal(&mutex2);
		sema_signal(&full2);
	}
}

void *consumer(){
	int i, item;
	for(i = 0; i<ITEM_COUNT; i++){
		sema_wait(&full2);
		sema_wait(&mutex2);
		item = buffer2_get();
		printf("consume %c\n", item);
		sema_signal(&mutex2);
		sema_signal(&empty2);
	}
}

int main(){
	pthread_t prd_tid, cpt_tid, csm_tid;
	
	sema_init(&mutex1, 1);
	sema_init(&mutex2, 1);
	sema_init(&empty1, CAPACITY-1);
	sema_init(&empty2, CAPACITY-1);
	sema_init(&full1, 0);
	sema_init(&full2, 0);
	
	pthread_create(&prd_tid, NULL, producer, NULL);
	pthread_create(&cpt_tid, NULL, computer, NULL);
	pthread_create(&csm_tid, NULL, consumer, NULL);
	
	pthread_join(prd_tid, NULL);
	pthread_join(cpt_tid, NULL);
	pthread_join(csm_tid, NULL);
	
	return 0;
}