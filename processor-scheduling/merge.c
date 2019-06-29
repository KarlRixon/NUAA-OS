#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #include <set>
#include <unistd.h>
#define R "RUN"    //������
#define F "FINISH" //�����
#define W "WAITE"  //�ȴ���
#define T "TAKEN"  //δ�ύ
#define MAX 5

struct PCB
{
    char Name[10];           //������
    int Arrive;              //����ʱ��
    int Need;                //��Ҫ����ʱ��
    int Start;               //��ʼʱ��
    int End;                 //���ʱ��
    int Turnover;            //��תʱ��
    int Priority;            //���ȼ�
    int CpuTime;             //����cpuʱ��
    float UseWeightTurnover; //��Ȩ��תʱ��
    char Status[10];         //����״̬
};

typedef struct PCB_
{
    int PID;
    char state;
    int priority;
    int runTime;
    int workTime;
    struct PCB_ *next;
} PCB_;
PCB_ *process, ptr;

PCB_ *ready = NULL, *p;
int num = 5;

int CurrentTime; //��ǰʱ��
int finish;      //���������
char c;          //�㷨ѡ��
int visit[MAX];  //����Ƿ���ڶ���

//����PCB
void CreatePCB(struct PCB *pcb)
{
    int fd_stdin = dup(fileno(stdin));
    freopen("./input.txt", "r", stdin);
    printf("���ļ��ж����ĸ����������ݣ�\n");
    printf("��ҵ�� ���ȼ� ����ʱ�� ��Ҫ����ʱ��\n");
    for (int i = 0; i < MAX; i++)
    {
        scanf("%s", &pcb[i].Name);     //������
        scanf("%d", &pcb[i].Priority); //���ȼ�
        scanf("%d", &pcb[i].Arrive);   //����ʱ��
        scanf("%d", &pcb[i].Need);     //��Ҫ����ʱ��
        pcb[i].Start = -1;
        pcb[i].End = -1;
        pcb[i].Turnover = 0;
        pcb[i].UseWeightTurnover = 0.0;
        pcb[i].CpuTime = 0;
        strcpy(pcb[i].Status, T);
        printf("%s\t%2d\t%2d\t%5d\n", pcb[i].Name, pcb[i].Priority, pcb[i].Arrive, pcb[i].Need);
    }
    fclose(stdin);
    fdopen(fd_stdin, "r");
    printf("---------------------------------------------\n");
}

typedef struct Node
{
    int data;          /*������*/
    struct Node *next; /*ָ����*/
} Node;
typedef struct
{
    Node *front;
    Node *rear;
} Queue;
void InitQueue(Queue *q)
{
    /* ��Q��ʼ��Ϊһ���յ������� */
    q->front = (Node *)malloc(sizeof(Node));
    if (q->front != NULL)
    {
        q->rear = q->front;
        q->front->next = NULL;
    }
}

/*��Ӳ�����*/
void Enqueue(Queue *q, int x)
{
    /* ������Ԫ��x���뵽����Q�� */
    Node *NewNode;
    NewNode = (Node *)malloc(sizeof(Node));
    if (NewNode != NULL)
    {
        NewNode->data = x;
        NewNode->next = NULL;
        q->rear->next = NewNode;
        q->rear = NewNode;
    }
}
/*���Ӳ�����*/
int Dequeue(Queue *q)
{
    /* ������Q�Ķ�ͷԪ�س��ӣ�����ŵ�x��ָ�Ĵ洢�ռ��� */
    Node *p;
    int x;
    p = q->front->next;
    q->front->next = p->next;
    if (q->rear == p)
        q->rear = q->front;
    x = p->data;
    free(p);
    return x;
}

//��ӡ
void Display(struct PCB *pcb)
{
    int i;
    printf("��ǰʱ��Ϊ%d\n", CurrentTime);
    printf("������ ���ȼ� ����ʱ�� ��Ҫ����ʱ�� ����cpuʱ�� ��ʼʱ�� ���ʱ�� ����״̬\n");
    for (i = 0; i < MAX; i++)
    {
        printf("%s\t%d\t%3d\t%4d\t\t%d\t%4d\t%6d\t   %s\n",
               pcb[i].Name, pcb[i].Priority, pcb[i].Arrive,
               pcb[i].Need, pcb[i].CpuTime, pcb[i].Start, pcb[i].End,
               pcb[i].Status);
    }
    printf("--------------------------------------------------------------------------\n");
}

//����״̬������޸�
void StatusConfirm(struct PCB *pcb)
{
    int i;
    for (i = 0; i < MAX; i++)
    { //����ʱ����ڵ�ǰʱ����״̬��Ϊ�ȴ���
        if (CurrentTime >= pcb[i].Arrive && strcmp(pcb[i].Status, T) == 0)
        {
            strcpy(pcb[i].Status, W);
        }
        if (pcb[i].CpuTime == pcb[i].Need && strcmp(pcb[i].Status, W) == 0)
        {
            finish++;
            strcpy(pcb[i].Status, F);
            pcb[i].End = CurrentTime;
            pcb[i].Turnover = pcb[i].End - pcb[i].Arrive;
            pcb[i].UseWeightTurnover = pcb[i].Turnover * 1.0 / pcb[i].Need;
        }
    }
}

//ȡ��������������ʱ����̵Ľ����±�
int ShortIndex(struct PCB *pcb)
{
    int i;
    int min;
    int temp;
    min = 100;
    temp = -1;
    StatusConfirm(pcb); //���½���״̬
    for (i = 0; i < MAX; i++)
    {
        if (strcmp(pcb[i].Status, W) == 0)
        {
            if (pcb[i].Need < min)
            { //�и�С������ʱ�������
                min = pcb[i].Need;
                temp = i;
            }
        }
    }
    return temp;
}

//�Ƚϸ�������֮��ĵ���ʱ��,����������
void Sort(struct PCB *pcb)
{
    int i;
    int j;
    int min;
    int minIndex;
    for (i = 0; i < MAX; i++)
    {
        min = pcb[i].Arrive;
        minIndex = i;
        for (j = i + 1; j < MAX; j++)
        {
            if (pcb[j].Arrive < min)
            {
                min = pcb[j].Arrive;
                minIndex = j;
            }
        }
        struct PCB temp = pcb[i];
        pcb[i] = pcb[minIndex];
        pcb[minIndex] = temp;
    }
}

//����ҵ����
void ShortRuntime(struct PCB *pcb)
{
    Sort(pcb);
    int index;
    int first;
    first = pcb[0].Arrive;
    for (index = 0, finish = 0; finish != MAX; CurrentTime++)
    { //���̵���λcurrentTimeÿ�μ�1��ֱ������ȫ�����������Ϊֹ
        StatusConfirm(pcb);
        if (CurrentTime < first || ShortIndex(pcb) == -1)
        { //��쵽��Ľ��̻�δ���������������޽���
           // Display(pcb);
        }
        else
        {
            index = ShortIndex(pcb);        //ȡ����ʱ����̽���
            //index = 0;
            pcb[index].Start = CurrentTime; //���¿�ʼ����ʱ��
            strcpy(pcb[index].Status, R);   //���½���״̬Ϊ������
            while (1)
            { //���д˽���
                StatusConfirm(pcb);
                if (pcb[index].Need == pcb[index].CpuTime)
                {                                 //�����õ�cpuʱ�������������ʱ��
                    strcpy(pcb[index].Status, F); //����״̬��Ϊ�����
                    finish++;
                    break;
                }
                else
                {
                    Display(pcb);
                    CurrentTime++;
                }
                pcb[index].CpuTime++; //����ʱ��Ƭ+1
                StatusConfirm(pcb);
            };
            pcb[index].End = CurrentTime;                                               //�������ʱ��
            pcb[index].Turnover = pcb[index].End - pcb[index].Arrive;                   //������תʱ��
            pcb[index].UseWeightTurnover = pcb[index].Turnover * 1.0 / pcb[index].Need; //�����Ȩ��תʱ��
            strcpy(pcb[index].Status, F);
            CurrentTime--;
        }
    }
    Display(pcb);
}

//ʱ��Ƭת�ַ�
void HighPriority(struct PCB *pcb)
{
    Queue LQ;
    InitQueue(&LQ); //��������
    Sort(pcb);      //������ʱ������
    int index;
    int first;
    int j;
    int i;
    first = pcb[0].Arrive;
    memset(visit, 0, sizeof(visit)); //�����������
    for (index = 0, finish = 0; finish != MAX; CurrentTime++)
    { //���̵���λcurrentTimeÿ�μ�1��ֱ������ȫ�����������Ϊֹ
        StatusConfirm(pcb);
        //getchar();
        if (CurrentTime < first || ShortIndex(pcb) == -1)
        { //��쵽��Ľ���δ�������������޽���
            Display(pcb);
        }
        else
        {
            if (CurrentTime == first)
            {
                Enqueue(&LQ, 0);
                visit[0] = 1;
            }
            for (i = 0, j = index + 1; i < MAX; i++, j++)
            { //���ύ�Ľ������
                if (j == MAX)
                {
                    j = 0;
                }
                if (!visit[j] && strcmp(pcb[j].Status, W) == 0)
                {
                    Enqueue(&LQ, j);
                    visit[j] = 1; //�Ѵ��ڶ���
                }
            }
            index = Dequeue(&LQ); //ȡ��ͷ
            visit[index] = 0;     //�����ڶ�����
            if (pcb[index].Start == -1)
            { //���½��̿�ʼ����ʱ��
                pcb[index].Start = CurrentTime;
            }
            strcpy(pcb[index].Status, R); //����״̬Ϊ������
            Display(pcb);
            pcb[index].CpuTime++; //��ǰ��������cpuʱ������
            if (pcb[index].CpuTime == pcb[index].Need)
            { //������cpuʱ���Ѵﵽ��������ʱ��
                strcpy(pcb[index].Status, F);
                pcb[index].End = CurrentTime + 1;                                           // �������ʱ��
                pcb[index].Turnover = pcb[index].End - pcb[index].Arrive;                   //������תʱ��
                pcb[index].UseWeightTurnover = pcb[index].Turnover * 1.0 / pcb[index].Need; //�����Ȩ��תʱ��
                finish++;
            }
            else
            {
                strcpy(pcb[index].Status, W); //ʱ��Ƭ����������±�Ϊ�ȴ�״̬
            }
        }
    }
    Display(pcb);
}

//����ƽ����Ȩ��תʱ��
float WeightTurnoverTimeCount(struct PCB *pcb)
{
    float sum = 0.0;
    for (int i = 0; i < MAX; i++)
    {
        sum += pcb[i].UseWeightTurnover;
    }
    return sum / MAX;
}

//����ƽ����תʱ��
float TurnOverTimeCount(struct PCB *pcb)
{
    float sum = 0.0;
    for (int i = 0; i < MAX; i++)
    {
        sum += pcb[i].Turnover;
    }
    return sum / MAX;
}

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
    int temp;

    int fd_stdin = dup(fileno(stdin));
    freopen("./input2.txt", "r", stdin);
    printf("���ļ��ж����������������ݣ�\n");
    printf("��ҵ�� ���ȼ� ��Ҫ����ʱ��\n");

    printf("����%d�����̵���Ϣ��PID�����ȼ�������ʱ�䣩\n", num);
    for (i = 0; i < num; i++)
    {
        p = (PCB_ *)malloc(sizeof(PCB_));
        scanf("%d", &p->PID);
        scanf("%d", &p->priority);
        scanf("%d", &temp);
        scanf("%d", &p->runTime);
        // scanf("%d %d %d %d", &p->PID, &p->priority, &temp, &p->runTime);
        p->workTime = 0;
        p->state = 'w';
        p->next = NULL;
        printf("%d\t%2d\t%5d\n", p->PID, p->priority, p->runTime);
        PCB_sort();
    }
    fclose(stdin);
    fdopen(fd_stdin, "r");
    printf("---------------------------------------------\n");
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
    printf("\nPID\t״̬\t���ȼ�\t����ʱ��\tʣ��ʱ��\n");
    printf("��������������������������������������������������������\n");
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
        printf("\n ���д���:%d \n", h);
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
            printf("\n ����%d �ѽ�����\n", p->PID);
            free(p);
        }
        else
        {
            (p->priority)++;
            p->state = 'w';
            PCB_sort();
        }
        printf("\n ����һ������ ......");
    }
    printf("\n\n �����Ѿ���� .\n");
    ch = getchar();
}

//��ʼ���̵���
void Start()
{
    int i;
    struct PCB pcb[MAX];
    printf("��ѡ����̵����㷨��\n1������ҵ���ȵ����㷨������ռ����2��ʱ��Ƭת�ֵ����㷨��3�����ȼ������㷨��\n\n");
    scanf("%c", &c);

    switch (c)
    {
    case '1':
        CreatePCB(pcb);
        ShortRuntime(pcb);
        break;
    case '2':
        CreatePCB(pcb);
        HighPriority(pcb);
        break;
    case '3':
        priorityRun();
        return;
    default:
        break;
    }
    printf("������ ��תʱ�� ��Ȩ��תʱ��\n");
    for (i = 0; i < MAX; i++)
    {
        printf("%s\t%d\t%.2f\n", pcb[i].Name, pcb[i].Turnover, pcb[i].UseWeightTurnover);
    }
    printf("---------------------------------------------\n");
    printf("ƽ����תʱ��Ϊ��%.2f\n", TurnOverTimeCount(pcb));
    printf("ƽ����Ȩ��תʱ��Ϊ��%.2f\n", WeightTurnoverTimeCount(pcb));
}

//������
int main()
{
    Start();
    return 0;
}
