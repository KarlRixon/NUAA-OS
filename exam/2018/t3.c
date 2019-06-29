//主线程创建10个子线程
//第0个子线程计算1加到10，第二个子线程计算11加到20，，，
//主线程对子线程计算结果求和，结果为5050
//本题使用线程参数完成

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define N 10
#define TOTAL 100

typedef struct param{
	int start;
	int end;
}param;

typedef struct result{
	int sum;
}result;

void *compute(void *arg){
	param *p;
	p = (param *)arg;
	int start = p->start;
	int end = p->end;
	int i,sum = 0;
	for(i = start; i<=end; i++)
		sum += i;
	result *r = malloc(sizeof(result));
	r->sum = sum;
	return r;
}

int main(){
	pthread_t workers[N];
	param p[N];
	int i, sum = 0;
	
	for(i = 0; i<N; i++){
		p[i].start = TOTAL/N*i + 1;
		p[i].end = p[i].start + 9;
		pthread_create(&workers[i], NULL, compute, &p[i]);
	}
	for(i = 0; i<N; i++){
		result *r;
		pthread_join(workers[i], (void **)&r);
		sum += r->sum;
		free(r);
	}
	printf("sum = %d\n", sum);
	return 0;
}