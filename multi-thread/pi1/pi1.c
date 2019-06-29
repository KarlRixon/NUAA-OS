#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int n;		// n表示数列求和的个数
// a_x = 2*x-1
void *compute(void *arg){
	int *a = (int *)arg;
	int i = 0;
	double *sum;
	*sum = 0;
	int k = 0;
	for(i = *a; i<n; i++){
		if((i+1)%2 == 0)
			k = (-1)*((i+1)*2-1);
		else
			k = ((i+1)*2-1);
		*sum += 1.0/(double)k;
	}
	return sum;
}

int main(){
	int mid = 0;
	scanf("%d", &n);
	mid = n/2;
	void *arg = &mid;
	pthread_t tid;
	pthread_create(&tid, NULL, compute, arg);
	
	int i = 0;
	double pi = 0;
	int k = 0;
	for(i = 0; i<mid; i++){
		if((i+1)%2 == 0)
			k = -1*((i+1)*2-1);
		else
			k = ((i+1)*2-1);
		pi += 1.0/(double)k;
	}
	
	double *r;
	pthread_join(tid, (void **)&r);
	printf("%.10lf\n", (pi+(*r))*4);
}