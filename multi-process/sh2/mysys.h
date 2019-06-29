#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

void split(char *command, int *argc, char **argv);
void mysys(char *command);
