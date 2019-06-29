#include "mysys.h"

static int command_size = 10;
static char dir[256];

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
	*argv = NULL;
	*argc = count;
}

void mysys(char *com){
	char *command = malloc(sizeof(char)*strlen(com));
	strcpy(command, com);
	int wordc = 0;
	char *wordv[command_size];
	split(command, &wordc, wordv);
	
	if(strcmp(wordv[0], "cd") == 0){
		sscanf(wordv[1], "%s", dir);
		chdir(dir);
	}
	else if(strcmp(wordv[0], "pwd") == 0){
		getcwd(dir, 256);
		puts(dir);
	}
	else{
		int pid = fork();
		if(pid == 0){
			int error = execvp(wordv[0], wordv);
			if(error < 0)
				perror("execvp");
			exit(0);
		}
		else
			wait(NULL);
	}
}
