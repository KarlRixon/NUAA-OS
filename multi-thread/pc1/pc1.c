#include<stdio.h>
#include<pthread.h>

#define CAPACITY 4
#define ITEM_COUNT 8
int buffer1[CAPACITY];
int buffer2[CAPACITY];
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

pthread_cond_t empty1, empty2;
pthread_cond_t full1, full2;
pthread_mutex_t mutex1, mutex2;

void *producer(void *arg){
	int i;
	for(i = 0; i<ITEM_COUNT; i++){
		pthread_mutex_lock(&mutex1);
		while(buffer1_is_full()){
			pthread_cond_wait(&empty1, &mutex1);
		}
		buffer1_put('a'+i);
		printf("produce %c\n", 'a'+i);
		
		pthread_cond_signal(&full1);
		pthread_mutex_unlock(&mutex1);
	}
	return;
}

void *computer(void *arg){
	int i, item;
	for(i = 0; i<ITEM_COUNT; i++){
		pthread_mutex_lock(&mutex1);
		while(buffer1_is_empty()){
			pthread_cond_wait(&full1, &mutex1);
		}
		item = buffer1_get();
		pthread_cond_signal(&empty1);
		pthread_mutex_unlock(&mutex1);
		
		item += 'A'-'a';
		
		pthread_mutex_lock(&mutex2);
		while(buffer2_is_full()){
			pthread_cond_wait(&empty2, &mutex2);
		}
		buffer2_put(item);
		printf("compute %c\n", item);
		pthread_cond_signal(&full2);
		pthread_mutex_unlock(&mutex2);
		
		//print_buffer();
	}
	return;
}

void *consumer(void *arg){
	int i;
	int item;
	for(i = 0; i<ITEM_COUNT; i++){
		pthread_mutex_lock(&mutex2);
		while(buffer2_is_empty()){
			pthread_cond_wait(&full2, &mutex2);
		}
		item = buffer2_get();
		printf("consume %c\n", item);
		
		pthread_cond_signal(&empty2);
		pthread_mutex_unlock(&mutex2);
	}
	return;
}

int main(){
	pthread_t prd_tid, cpt_tid, csm_tid;
	pthread_cond_init(&empty1, NULL);
	pthread_cond_init(&empty2, NULL);
	pthread_cond_init(&full1, NULL);
	pthread_cond_init(&full2, NULL);
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	
	pthread_create(&prd_tid, NULL, producer, NULL);
	pthread_create(&cpt_tid, NULL, computer, NULL);
	pthread_create(&csm_tid, NULL, consumer, NULL);
	
	pthread_join(prd_tid, NULL);
	pthread_join(cpt_tid, NULL);
	pthread_join(csm_tid, NULL);
	
	return 0;
}