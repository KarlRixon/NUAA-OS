#include <stdio.h>

typedef char *va_list;

#define va_start(ap, last_arg) 					                       \
    (ap = ((char *)&(last_arg) + sizeof(last_arg)))

#define va_end(ap)

#define va_arg(ap, type)					                           \
    (ap += sizeof(type),				                               \
    *((type *)(ap - sizeof(type))))

int add(char *fmt, ...)
{
	if(!strstr(fmt, "%") || sizeof(fmt) == 0){
		puts("no input");
		return 0;
	}
	char *format = (char *)malloc(sizeof(char *)*strlen(fmt));
	strcpy(format, fmt);
	
	va_list ap;
	int sum = 0;
	char *pNext = strtok(format, "%");
	va_start(ap, fmt);
	while(pNext != NULL){
		if(pNext[0] == 'd'){
			sum += va_arg(ap, int);
		}
		else if(pNext[0] == 's'){
			sum += atoi(va_arg(ap, char *));
		}
		else{
			puts("format error");
			return 0;
		}
		pNext = strtok(NULL, "%");
	}
	va_end(ap);
	return sum;
}

int main()
{
    printf("sum = %d\n", add("%d %d", 11, 22));
    printf("sum = %d\n", add("%s %s", "11", "22"));
    printf("sum = %d\n", add("%d %s", 11, "22"));
    return 0;
}