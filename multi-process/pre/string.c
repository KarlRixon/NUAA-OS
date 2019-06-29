#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void split(char *command, int *argc, char *argv[]){
	int count = 0;
	char *pNext = NULL;
	if(command == NULL || strlen(command) == 0)
		return;
	pNext = strtok(command, " ");
	while(pNext != NULL){
		//printf("%s\n", pNext);
		*argv++ = pNext;
		++count;
		pNext = strtok(NULL, " ");
	}
	*argc = count;
}

void test(){
	int wordc;
        char *wordv[8] = {0};
        char command[] = "echo a b";
        split(command, &wordc, wordv);
        int i;
        for(i = 0; i < wordc; i++)
                puts(wordv[i]);
}

int main(){
	//int wordc;
	//char *wordv[8] = {0};
	//char command[] = "echo a b";
	//split(command, &wordc, wordv);
	//int i;
	//for(i = 0; i < wordc; i++)
	//	puts(wordv[i]);
	test();
	return 0;
}
