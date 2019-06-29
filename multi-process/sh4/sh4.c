#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

#define MAX_ARGS 8
#define MAX_COMMAND 8

static char *command = NULL;
static int line_size = 256;
static int fdin, fdout, flag;	// flag为0表示管道分割出的第一条命令，为1表示最后一条，为2表示中间
static int fd[2], fd_temp[2], recover[2];

typedef struct cmd{
	int argc;
	char *argv[MAX_ARGS];
	char *input;
	char *output;
}cmd;

void print_parse(cmd *com);

void readline(char *prompt){
	printf("%s", prompt);
	command = (char *)malloc(sizeof(char)*line_size);
	char ch;
	int count = 0;
	while((ch = getchar()) != '\n'){
		*(command + count) = ch;
		count += 1;
	}
	*(command + count) = '\0';
}

// 用' '分割出一条命令的指令和参数
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

// 字符串到结构体的处理
void str2cmd(char *str, cmd *p){
	int i;
	char *pNext, *pos;
	if(str == NULL || strlen(str) == 0)
		return;
	if(!strstr(str, ">") && !strstr(str, "<")){
		p->input = NULL;
		p->output = NULL;
	}
	else{
		if(strstr(str, "<")){
			pos = strchr(str, '<');
			pos[0] = '\0';
			for(i = 1;;i++)
				if(pos[i] != ' '){
					pNext = pos+i;
					break;
				}
			p->input = strtok(pNext, " ");
			// 为下面做准备，这里容易出错
			for(i=0;;i++)
				if(pNext[i] == '\0'){
					pNext += i;
					break;
				}
			pNext += 1;
		}
		if(strstr(str, ">") || strstr(pNext, ">")){
			if(strstr(str, ">")){
				pos = strchr(str, '>');
			}
			else{
				pos = strchr(pNext, '>');
			}
			pos[0] = '\0';
			for(i = 1;;i++)
				if(pos[i] != ' '){
					pNext = pos+i;
					break;
				}
			p->output = strtok(pNext, " ");
		}
	}
	split(str, &p->argc, p->argv);
	//print_parse(p);
}

// 使用strtok有未知bug，只好自己写一个
// 将pNext中第一个分割出来的字符串拷到p，并将pNext后移
void my_strtok(char **p, char **pNext, char ch){		// 使用二级指针才能保存修改
	char *temp = NULL;
	int i;
	for(i = 0; (*pNext)[i] != '\0' ;i++){
		if((*pNext)[i] == ch){
			temp = (*pNext)+i+1;
			(*pNext)[i] = '\0';
			break;				// 这里忘记了加break
		}
	}
	*p = *pNext;
	
	if(temp == NULL){
		*pNext = NULL;
		return;
	}
	else{
		for(i = 0;;i++)			// 忽略空格
			if(temp[i] != ' ')
				break;
		temp += i;
		
		*pNext = temp;
		return;
	}
}

// 用'|'分割出多条命令，'|'最好用空格隔开
void parse(char *line, int *commandc, cmd **commands){
	int count = 0, i;
	char *p = NULL;
	if(line == NULL || strlen(line) == 0)
		return;
	
	char *pNext = line;
	
	while(1){
		my_strtok(&p, &pNext, '|');
		str2cmd(p, commands[count]);		// 会在原字符串添加一些'\0'来分割
		//print_parse(commands[count]);
		++count;
		if(pNext == NULL)
			break;
	}
	*commandc = count;
}

// 打印一条指令信息
void print_parse(cmd *com){
	printf("argc: %d\n", com->argc);
	int i;
	for(i = 0; i<com->argc; i++){
		printf("argv[%d]: %s\n", i, com->argv[i]);
	}
	printf("input: %s\n", com->input);
	printf("output: %s\n", com->output);
}

// 执行单独的一个管道命令
void pipe_sys(cmd *com){
	pid_t pid = fork();
	if(pid == 0){
		if(flag == 0){
			if(com->input != NULL)					// 输入重定向
				if((fdin = open(com->input, O_RDONLY, 0666)) == -1){
					puts("no such file or directory");
					exit(1);
				}
				else{
					dup2(fdin, 0);
					close(fdin);
				}
			else
				dup2(recover[0], 0);
			dup2(fd[1], 1);							// 输出重定向
			close(fd[0]);
			close(fd[1]);
			//puts("test_pipe");
			execvp(com->argv[0], com->argv);
			exit(0);
		}
		else if(flag == 1){
			dup2(fd[0], 0);							// 输入重定向
			close(fd[0]);
			close(fd[1]);
			if(com->output != NULL){				// 输出重定向
				fdout = open(com->output, O_RDWR|O_CREAT|O_TRUNC, 0666);
				dup2(fdout, 1);
				close(fdout);
			}
			else{
				dup2(recover[1], 1);
			}
			//char str[10];
			//scanf("%s", str);
			//puts(str);
			execvp(com->argv[0], com->argv);
			exit(0);
		}
		else if(flag ==2){
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			// 输出到临时管道
			dup2(fd_temp[1], 1);
			close(fd_temp[0]);
			close(fd_temp[1]);
			//char str[10];
			//scanf("%s", str);
			//puts(str);
			execvp(com->argv[0], com->argv);
			exit(0);
		}
	}
	wait(NULL);
	return;
}

// 处理并执行指令
void mysys(char *com){
	pid_t pid;
	int i, commandc, fd_chgr[2];
	cmd *commands[MAX_COMMAND];
	for(i = 0; i<MAX_COMMAND; i++)
		commands[i] = (cmd *)malloc(sizeof(cmd));
	char *c = (char *)malloc(sizeof(char)*strlen(com));
	strcpy(c, com);
	parse(c, &commandc, commands);
	//for(i = 0; i<commandc+1; i++){
	//	print_parse(commands[i]);
	//}
	
	pipe(fd);
	pipe(fd_temp);
	recover[0] = dup(0);
	recover[1] = dup(1);
	
	if(commandc == 1){		// 没有管道的情况
		if(pid = fork() == 0){
			if(commands[0]->input != NULL)			// 输入重定向
				if((fdin = open(commands[0]->input, O_RDONLY, 0666)) == -1){
					puts("no such file or directory");
					exit(1);
				}
				else{
					dup2(fdin, 0);
					close(fdin);
				}
			else
				dup2(recover[0], 0);
			if(commands[0]->output != NULL){		// 输出重定向
				fdout = open(commands[0]->output, O_RDWR|O_CREAT|O_TRUNC, 0666);
				dup2(fdout, 1);
				close(fdout);
			}
			else{
				dup2(recover[1], 1);
			}
			//print_parse(commands[0]);
			//run_cmd(commands[0]->argc, commands[0]->argv);
			execvp(commands[0]->argv[0], commands[0]->argv);
		}
		else{
			wait(NULL);
		}
	}
	else{			// 有管道的情况		/////////////////////////////////////bug
		flag = -1;
		for(i = 0; i<commandc; i++){
			if(flag == 2){
				dup2(fd_temp[0], fd[0]);
				dup2(fd_temp[1], fd[1]);
				close(fd_temp[0]);
				close(fd_temp[1]);
				pipe(fd_temp);
				close(fd[1]);
			}
			if(flag == 0)
				close(fd[1]);
			if(i == 0)
				flag = 0;
			else if(i == commandc-1)
				flag = 1;
			else
				flag = 2;
			pipe_sys(commands[i]);
		}
	}
	exit(0);
}
// 没有实现识别输出重定向前的文件名
int main(){
	char dir[256];
	while(1){
		getcwd(dir, 254);
		strcat(dir, "\033[34;1m > \033[0m");
		if(command){
			free(command);
			command = NULL;
		}
		readline(dir);
		if(strstr(command, "cd ")){		// 注意cd后面加了一个空格
			if(strlen(command) > 3){
				sscanf(command, "cd %s", dir);
				chdir(dir);
			}
		}
		else if(strcmp(command, "pwd") == 0){
			getcwd(dir, 256);
			puts(dir);
		}
		else if(strcmp(command, "exit") == 0)
			exit(0);
		else{
			pipe(fd);
			pipe(fd_temp);
			recover[0] = dup(0);
			recover[1] = dup(1);
			int pid = fork();
			if(pid == 0){
				mysys(command);
			}
			else wait(NULL);
		}
	}
}