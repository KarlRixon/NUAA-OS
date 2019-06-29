#include "mysys.h"

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
	*argv = NULL;
	*argc = count;
}

void mysys(char *com){
	int error = -1;
	char *command = malloc(sizeof(char)*strlen(com));
	strcpy(command, com);
	int wordc = 0;
	char *wordv[command_size];
	split(command, &wordc, wordv);
	if(wordv[2] && (strcmp(wordv[2], ">") == 0)){
		int fd = open(wordv[3], O_CREAT|O_RDWR|O_TRUNC, 0666);
		dup2(fd, 1);
		close(fd);
		write(1, wordv[1], strlen(wordv[1]));
		error = 0;
	}
	else if(wordv[2] && (strcmp(wordv[2], "|") == 0)){
		int pid;
		int fd[2];

		pipe(fd);
		pid = fork();
		if(pid == 0){
			dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			execlp(wordv[0], wordv[0], wordv[1], NULL);	// improvement needed
			exit(0);
		}
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execlp(wordv[3], wordv[3], wordv[4], NULL);		// improvement needed
		return;
	}
	else{
		error = execvp(wordv[0], wordv);
	}

	//int i;
	//for(i=0; i<command_size; i++){
	//	printf("%d ", i);
	//	puts(wordv[i]);
	//}
	
	if(error < 0)
		perror("execvp");
	exit(0);
}
