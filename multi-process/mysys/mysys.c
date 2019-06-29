#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

static int command_size = 10;

void split(char *command, int *argc, char **argv){
	int count = 0;
	if(command == NULL || strlen(command) == 0)
		return;
	char *pNext = strtok(command, " ");
	while(pNext != NULL){
		*argv++ = pNext;
		++count;
		pNext = strtok(NULL, " ");
	}
	*argv = NULL;		//for execvp use
	*argc = count;
}
void mysys(char *com){
	char *command = malloc(sizeof(char)*strlen(com));
	strcpy(command, com);
	int wordc = 0;
	char *wordv[command_size];
	split(command, &wordc, wordv);
	int pid = fork();
    if(pid == 0){
        int error = execvp(wordv[0], wordv);
	    if(error < 0)
	    	perror("execvp");
        exit(0);
    }
    wait(NULL);
}

int main(){
    printf("-----------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("-----------------------------------------\n");
    mysys("ls /");
    printf("-----------------------------------------\n");
    return 0;
}
