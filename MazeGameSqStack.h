#include <stdlib.h>
#define STACK_INIT_SIZE 1050
#define STACKINCREMENT 10
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW 0
#define OK 1

typedef int Status;
typedef struct PosType
{
	int x;
	int y;
} PosType;
typedef struct SElemType
{
	PosType seat; //坐标
	int di;		  //方向
} SElemType;
typedef struct SqStack
{
	SElemType* base;
	SElemType* top;
	int stacksize;
} SqStack;

Status GetTop(SqStack* s, SElemType* e)	//获取栈顶元素
{
	if ((*s).top == (*s).base)
	{
		return ERROR;
	}
	*e = *((*s).top - 1);
	return OK;
}
Status InitStack(SqStack* s)	//构造一个空栈
{
	(*s).base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!(*s).base)
	{
		return OVERFLOW;
	}
	(*s).top = (*s).base;
	(*s).stacksize = STACK_INIT_SIZE;
	return OK;
}
Status Pop(SqStack* s, SElemType* e)	//出栈
{
	if ((*s).top == (*s).base)
	{
		return ERROR;
	}
	*e = *--(*s).top;
	return OK;
}
Status Push(SqStack(*s), SElemType e)	//入栈
{
	if ((*s).top - (*s).base >= (*s).stacksize)
	{
		(*s).base = (SElemType*)realloc((*s).base, ((*s).stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!(*s).base)
		{
			exit(OVERFLOW);
		}
		(*s).top = (*s).base + (*s).stacksize;
		(*s).stacksize += STACKINCREMENT;
	}
	*(*s).top++ = e;
	return OK;
}
Status StackEmpty(SqStack s)	//栈是否为空
{
	if (s.top == s.base)
	{
		return OK;
	}
	return OVERFLOW;
}
void StackClear(SqStack* s)	//清空栈里所有元素
{
	SElemType temp;
	while (!StackEmpty((*s)))
	{
		Pop(s, &temp);
	}
}
