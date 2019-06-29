#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 100

int numbers[SIZE];
int result[SIZE];

void select_sort(int start, int end){
	int i, j;
	for(i = start; i<end; i++){
		int min = i;
		for(j = i; j<=end; j++)
			if(numbers[j] < numbers[min])
				min = j;
		int temp = numbers[i];
		numbers[i] = numbers[min];
		numbers[min] = temp;
	}
}

void *start_thread(void *arg){
	select_sort(SIZE/2+1, SIZE-1);
	
	return NULL;
}

int main(){
	srand(time(NULL));
	
	// 随机赋初值
	int i = 0;
	for(i = 0; i<SIZE; i++){
		numbers[i] = rand()%200;
	}
	
	// 创建线程
	pthread_t tid;
	pthread_create(&tid, NULL, start_thread, NULL);
	
	// 排序前半部分
	select_sort(0, SIZE/2);
	
	// 等待线程
	pthread_join(tid, NULL);
	
	// 归并排序 0-(SIZE/2) & (SIZE/2+1)-SIZE
	int p = 0, q = SIZE/2+1, r = 0;
	while(p <= SIZE/2 || q < SIZE){
		if(p > SIZE/2 && q < SIZE) result[r++] = numbers[q++];
		if(p <= SIZE/2 && q >= SIZE) result[r++] = numbers[p++];
		if(p <= SIZE/2 && q < SIZE)
			result[r++] = numbers[p] < numbers[q] ? numbers[p++] : numbers[q++];
	}
	
	// 输出排序结果
	for(i = 0; i<SIZE; i++){
		printf("%d\t", result[i]);
	}
	puts("");
	
	return 0;
}