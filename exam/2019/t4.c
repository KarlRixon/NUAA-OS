//现有一个水果盘，一次只能放一个水果
//父亲一次向里边放一个苹果，母亲一次向里边放一个橘子
//儿子一次从盘中取出一个苹果，女儿一次从盘中取出一个橘子
//要求只能使用条件变量，不能使用环境变量
//输出结果为
//put apple
//get apple
//put orange
//get orange

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int plane = 1;//1表示盘子空 0表示盘子满

pthread_mutex_t mutex;
pthread_cond_t wait_apple;
pthread_cond_t wait_empty;
pthread_cond_t wait_orange;

void *father_entry(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(plane == 0)
			pthread_cond_wait(&wait_empty, &mutex);
		puts("put apple");
		pthread_cond_signal(&wait_apple);
		sleep(1);
		plane = 0;
		
		pthread_mutex_unlock(&mutex);
		
	}
	return NULL;
}

void *mother_entry(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(plane == 0)
			pthread_cond_wait(&wait_empty, &mutex);
		
		puts("put orange");
		pthread_cond_signal(&wait_orange);
		sleep(1);
		plane = 0;
		
		pthread_mutex_unlock(&mutex);
		
    
	}
	return NULL;
}

void *son_entry(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(plane == 1)
			pthread_cond_wait(&wait_apple, &mutex);

		puts("get apple");
		pthread_cond_signal(&wait_empty);
		sleep(1);
		plane = 1;
		
		pthread_mutex_unlock(&mutex);
		
    
	}
	return NULL;
}

void *daughter_entry(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(plane == 1)
			pthread_cond_wait(&wait_orange, &mutex);

		puts("get orange");
		pthread_cond_signal(&wait_empty);
		sleep(1);
		plane = 1;
		
		pthread_mutex_unlock(&mutex);
		
    }
	return NULL;
}

int main()
{
    pthread_t father_thread;
    pthread_t mother_thread;
    pthread_t son_thread;
    pthread_t daughter_thread;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&wait_empty, NULL);
	pthread_cond_init(&wait_apple, NULL);
	pthread_cond_init(&wait_orange, NULL);

    pthread_create(&father_thread, NULL, &father_entry, NULL);
    pthread_create(&mother_thread, NULL, &mother_entry, NULL);
    pthread_create(&son_thread, NULL, &son_entry, NULL);
    pthread_create(&daughter_thread, NULL, &daughter_entry, NULL);

    while (1);
    return 0;
}
