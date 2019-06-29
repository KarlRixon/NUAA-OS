#include <stdio.h>
#include <stdlib.h>

typedef struct PCB_
{
    int PID;
    char state;
    int priority;
    int runTime;
    int workTime;
    struct PCB_ *next;
}PCB_;
PCB_ * process, ptr;

PCB_ *ready = NULL, *p;
int num;

void PCB_sort()
{
    PCB_ *first, *second;
    int flag = 0;
    if ((ready == NULL) || ((p->priority) < (ready->priority)))
    {
        p->next = ready;
        ready = p;
    }
    else
    {
        first = ready;
        second = first->next;
        while (second != NULL)
        {
            if ((p->priority) < (second->priority))
            {
                p->next = second;
                first->next = p;
                second = NULL;
                flag = 1;
            }
            else
            {
                first = first->next;
                second = second->next;
            }
        }
        if (flag == 0)
            first->next = p;
    }
}

void inputProcess()
{
    int i;
    printf("输入%d个进程的信息（PID、优先级、运行时间）\n", num);
    for (i = 0; i < num; i++)
    {
        p = (PCB_ *)malloc(sizeof(PCB_));
        scanf("%d %d %d", &p->PID, &p->priority, &p->runTime);
        p->workTime = 0;
        p->state = 'w';
        p->next = NULL;
        PCB_sort();
    }
}

int space()
{
    int k = 0;
    PCB_ *pr = ready;
    while (pr != NULL)
    {
        k++;
        pr = pr->next;
    }
    return k;
}

void showInfo(PCB_ *pr)
{
    printf("\nPID\t状态\t优先级\t运行时间\t剩余时间\n");
    printf("————————————————————————————\n");
    printf(" %d\t", pr->PID);
    printf(" %c\t", pr->state);
    printf("%d\t", pr->priority);
    printf("%d\t\t", pr->runTime);
    printf("%d\t", pr->runTime - pr->workTime);
    printf("\n");
}

void priorityRun()
{
    int len, h = 0;
    char ch;
    inputProcess();
    len = space();
    while ((len != 0) && (ready != NULL))
    {
        ch = getchar();
        h++;
        printf("\n 运行次数:%d \n", h);
        p = ready;
        ready = p->next;
        p->next = NULL;
        p->state = 'R';
        PCB_ *pr;
        showInfo(p);
        pr = ready;
        while (pr != NULL)
        {
            showInfo(pr);
            pr = pr->next;
        }
        (p->workTime)++;
        if (p->workTime == p->runTime)
        {
            printf("\n 进程%d 已结束。\n", p->PID);
            free(p);
        }
        else
        {
            (p->priority)++;
            p->state = 'w';
            PCB_sort();
        }
        printf("\n 按任一键继续 ......");
    }
    printf("\n\n 进程已经完成 .\n");
    ch = getchar();
}

int main()
{
    printf("—————————————————优先级调度算法—————————————————\n");
    printf("输入进程数目：");
    scanf("%d", &num);
    priorityRun();
}
