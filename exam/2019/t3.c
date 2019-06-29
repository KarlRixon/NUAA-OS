//主进程创建2个子线程，分别计算1-50,51-100的累加和
//不能使用任何全局变量
//创建线程在parallel_add函数中填写

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct param{
	int start;
	int end;
}param;

typedef struct result{
	int sum;
}result;

void *compute(void *arg){
	param *p = (param *)arg;
	int start =  p->start;
	int end = p->end;
	int i, sum = 0;
	for(i = start; i<=end; i++)
		sum += i;
	result *r = (result *)malloc(sizeof(result));
	r->sum = sum;
	return r;
}

int parallel_add(int start, int end)
{
	int sum = 0;
	
	pthread_t workers[2];
	param p[2];

	p[0].start = start;
	p[0].end = (start+end)/2-1;
	p[1].start = p[0].end+1;
	p[1].end = end-1;
	pthread_create(&workers[0], NULL, compute, &p[0]);
	pthread_create(&workers[1], NULL, compute, &p[1]);
	result *r1, *r2;
	pthread_join(workers[0], (void **)&r1);
	pthread_join(workers[1], (void **)&r2);
	sum = r1->sum+r2->sum;
	return sum;
}

int main()
{
    // 1+2+3 ... + 100 = 5050
    printf("sum = %d\n", parallel_add(1, 101));
    return 0;
}
