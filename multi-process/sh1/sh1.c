#include"mysys.h"
#include<readline/readline.h>
#include<readline/history.h>

static char *command = NULL;

int main(){
	while(1){
		printf("> ");
		if(command){
			free(command);
			command = NULL;
		}
		command = readline("$ ");
		if(command && *command)
			add_history(command);
		if(strcmp(command, "exit") == 0)
			exit(0);
		else{
			mysys(command);
		}
	}
}
