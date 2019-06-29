#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

#define WORKERS 4

// a_x = 2*x-1

typedef struct param{
	int start;
	int end;
}param;

typedef struct result{
	double sum;
}result;

void *compute(void *arg){
	param *p = (param *)arg;
	int start = p->start;
	int end = p->end;
	double sum = 0;
	int i = 0;
	int k = 0;
	for(i = start; i<=end; i++){
		if(i%2 == 0)
			k = (-1)*(i*2-1);
		else
			k = (i*2-1);
		sum += 1.0/(double)k;
	}
	result *r = (result *)malloc(sizeof(result));
	r->sum = sum;
	return r;
}

int main(){
	int i = 0;
	int n;		// n表示数列求和的个数
	scanf("%d", &n);
	int start[WORKERS];
	int end[WORKERS];
	int average = n/WORKERS;
	int temp = 1;
	for(i = 0; i<WORKERS; i++){
		start[i] = temp;
		end[i] = start[i]+average;
		temp += (average+1);
	}
	end[WORKERS-1] = n;
	
	// 创建线程
	pthread_t tid[WORKERS];
	for(i = 0; i<WORKERS; i++){
		param *arg = (param *)malloc(sizeof(param));
		arg->start = start[i];
		arg->end = end[i];
		
		pthread_create(&tid[i], NULL, compute, arg);
	}
	
	// 求和
	double sum = 0;
	result *r;
	for(i = 0; i<WORKERS; i++){
		pthread_join(tid[i], (void **)&r);
		sum += ((result *)r)->sum;
		// printf("%f\n", ((result *)r)->sum);
	}
		
	printf("%.10lf\n", sum*4);
}