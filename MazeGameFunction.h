// ===== 头文件 =====

#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdbool.h>
#include <conio.h>
#include <time.h>	//获取当前时间
#include "MazeGame_Print_Color.h"	//改变颜色和显示效果
#include "MazeGame_SqStack.h"	//栈存储结构相关的函数

// ===== 只有一条路径的函数定义及变量定义 =====

#define start_x 2	//起点
#define start_y 2

	// ----- 全局变量定义 -----

using namespace std;
int X, Y;
SqStack Route;

	// ----- 函数原型声明 -----

int status = InitStack(&Route);
void create_mg(int mg[], int mglx[], int X, int Y);	//随机生成迷宫
void copy_mg(int mg[], int mglx[], int X, int Y);	//复制迷宫
void play_mg(int mg[], int mglx[], int X, int Y);	//完成迷宫路线求解
void show_Route(int mg[], int mglx[], int X, int Y);	//坐标路线展示
void show_mg(int mg[], int mglx[], int X, int Y);	//迷宫路线展示

// ===== 多条路径的函数定义及变量定义 =====

	// ----- 迷宫和路径数组中的数字 -----

#define Wall 1							//在Maze数组中1代表墙，0代表路
#define Way 0
#define Right 1							//Right,Down,Left,Up表示遍历方向
#define Down 2
#define Left 3
#define Up 4
#define None 0							//表示路径数组中这一个格子已经遍历过
#define Max_Row 50						//行数最大值
#define Max_Column 50					//列数最大值
#define Min_Row 5
#define Min_Column 5
#define PASS 1
#define FAIL -1							//表示这个格子走不了
#define END 5							//标志路径数组中的终点
#define Dynamic 1						//动态显示路径
#define Non_Dynamic 0

	// ----- 判断条件 -----

										//dirr[i]是对称的，但是在整数范围内，dirr[i]只能在{1,2,3,4}中选择，且每个dirr[i]不能重复
#define Judge_Input_Direction dirr[1] * dirr[2] * dirr[3] * dirr[4] == 24 && abs(dirr[1] - 2) + abs(dirr[2] - 2) + abs(dirr[3] - 2) + abs(dirr[4] - 2) == 4
										//周围都是墙
#define Surround_All_Walls Maze[i][j] == Way && Maze[i + 1][j] == Wall && Maze[i - 1][j] == Wall && Maze[i][j + 1] == Wall && Maze[i][j - 1] == Wall
										//周围都是路
#define Surround_All_Ways Maze[i][j] == Way && Maze[i + 1][j] == Way && Maze[i - 1][j] == Way && Maze[i][j + 1] == Way && Maze[i][j - 1] == Way && Maze[i + 1][j + 1] == Way && Maze[i - 1][j - 1] == Way && Maze[i + 1][j - 1] == Way && Maze[i - 1][j + 1] == Way

	// ----- 菜单显示选项 -----

#define Input_ERROR 0	//非法输入
#define Input_Maze_Size 1	//设定迷宫大小
#define Random_CreateMaze_First 2	//随机生成迷宫
#define Back_Menu 3	//返回菜单
#define Exit_MazeGame 4	//退出程序
#define Maze_Non_Solution 5	//迷宫无解
#define Table 6	//开始菜单的选项

	// ----- 全局变量定义 -----

int M, N;								//M代表迷宫的行数，N代表迷宫的列数
int P = 7;								//随机生成迷宫的路和墙的期望比值(P:10-P)
int Maze[Max_Row + Min_Row][Max_Column + Min_Column];	//Maze数组用于存储迷宫的墙和路，还可以加入其他的元素
int vis[Max_Row + Min_Row][Max_Column + Min_Column];	//当vis数组不为0时，用于存储路径的方向，为0则表示未走过
int count_route = 0;							//路径总数
int dirr[5];							//遍历顺序
int minlength = (Max_Row + Min_Row) * (Max_Column + Min_Column);	//目前最小的路径长度
int maxread;							//最大阅读路径数量
char ch;
int non_solution = FALSE;	//non_solution判断迷宫是否有解

	// ----- 函数原型声明 -----

void gotoxy(int x, int y);	//将光标移动到（x,y）位置
char MenuTable(int i);	//菜单选项的屏幕输出显示
void Print_RandomMaze();	//在屏幕上打印迷宫
void Random_CreateMaze();	//随机生成迷宫
Status Judge(PosType e);	//判断是否可走
PosType NextPos(PosType e, int dir);	//下一步
void PrintPath(SqStack* s);	//打印路径，由于入栈时是先入后出，但是路径打印要先入先出按照从入口到出口打印，所以要用一个中间栈来改变输出顺序
void PrintMaze();	//打印迷宫
void DFS(PosType start, PosType end, SqStack* s, int sta);	//sta=0时,检测是否可通；sta=1时，动态显示路径
void MulDfs(PosType start, PosType end, SqStack* s, int len);	//深度搜索所有路径
void BestDfs(PosType start, PosType end, SqStack* s, int len);	//深度搜索最短路径
void Get_MazeSize();	//获取迷宫的长度和宽度
void Get_Route_Numb();	//输入查看路径的数量
void Get_Maze_Solution(PosType start, PosType end, SqStack S);	//判断是否生成无解的迷宫
void Reinitialize_Variable(SqStack S);	//重新初始化路径数组、count_route、栈以及返回菜单
void Change_Order();	//改变遍历的方向

// ===== 多条路径的函数原型及主函数 =====

	// ----- 主要函数 -----

void MazeGame_Random()	//随机生成迷宫主程序
{
	int choice[4];	//choice用于判断用户选项顺序是否正确
	memset(choice, 0, sizeof(choice));	//初始化choice
	for (int i = 1; i <= 4; i++)	//遍历方向初始化
	{
		dirr[i] = i;
	}
	SqStack S;
	InitStack(&S);
	PosType start, end;	//设定迷宫的起点和终点
	while (1)	//多次返回主菜单
	{
		char opt = MenuTable(Table);	//显示菜单
		switch (opt)	//opt用于记录用户的选项
		{
		case '1':	//1.设定迷宫大小
		{
			choice[1] = 1;	//标记已经设定了迷宫大小
			Get_MazeSize();	//获取迷宫的行数和列数
			start.x = 1;
			start.y = 0;
			end.x = M - 2;
			end.y = N - 1;
			puts("迷宫设置完毕！");
			MenuTable(Back_Menu);
			break;
		}
		case '2':	//2.自动生成迷宫
		{
			system("cls");
			if (choice[1] == 0)
			{
				MenuTable(Input_Maze_Size);
				continue;
			}
			else	//随机生成迷宫
			{
				choice[2] = 1;	//标记已经随机生成迷宫
				Get_Maze_Solution(start, end, S);	//判断生成的迷宫类型
				break;
			}
		}
		case '3':	//3.修改遍历顺序
		{
			Change_Order();
			break;
		}
		case '4':	//4.动态显示路径
		{
			system("cls");
			if (choice[1] == 0)
			{
				MenuTable(Input_Maze_Size);
				continue;
			}
			if (choice[2] == 0)
			{
				MenuTable(Random_CreateMaze_First);
				continue;
			}
			DFS(start, end, &S, 1);
			puts("动态显示完毕！");
			if (non_solution == Maze_Non_Solution)
			{
				MenuTable(Maze_Non_Solution);
				continue;
			}
			Reinitialize_Variable(S);
			break;
		}
		case '5':	//5.输出所有路径
		{
			system("cls");
			if (choice[1] == 0)
			{
				MenuTable(Input_Maze_Size);
				continue;
			}
			if (choice[2] == 0)
			{
				MenuTable(Random_CreateMaze_First);
				continue;
			}
			if (non_solution == Maze_Non_Solution)
			{
				MenuTable(Maze_Non_Solution);
				continue;
			}
			Get_Route_Numb();
			Print_RandomMaze();
			puts("");
			MulDfs(start, end, &S, 0);
			if (count_route == maxread)
			{
				printf("\n已显示%d条路径，剩余路径省略！\n", maxread);
			}
			Reinitialize_Variable(S);
			break;
		}
		case '6':	//6.输出最短路径
		{
			system("cls");
			if (choice[1] == 0)
			{
				MenuTable(Input_Maze_Size);
				continue;
			}
			if (choice[2] == 0)
			{
				MenuTable(Random_CreateMaze_First);
				continue;
			}
			if (non_solution == Maze_Non_Solution)
			{
				MenuTable(Maze_Non_Solution);
				continue;
			}
			minlength = (Max_Row + Min_Row) * (Max_Column + Min_Column);	//最坏的情况下路径所需要走的长度
			Get_Route_Numb();
			Print_RandomMaze();
			BestDfs(start, end, &S, 0);
			printf("一共有%d条最短路；最短的路径长度为%d。\n", count_route, minlength);
			if (count_route > maxread)
			{
				printf("\n已显示%d条最短路，剩余路径省略！\n", maxread);
			}
			Reinitialize_Variable(S);
			break;
		}
		case '7':	//7.退出迷宫程序
		{
			system("cls");
			MenuTable(Exit_MazeGame);
			return;
		}
		default:
			continue;
		}
	}
}

	// ----- 函数原型定义 -----

char MenuTable(int i)	//菜单选项的屏幕输出显示
{
	fflush(stdin);
	switch (i)
	{
	case Input_ERROR:
		puts("输入非法，请重新输入！");
		break;
	case Input_Maze_Size:
		puts("请先设定迷宫大小！");
		break;
	case Random_CreateMaze_First:
		puts("请先随机生成迷宫！");
		break;
	case Back_Menu:
		puts("请按任意键返回主菜单");
		break;
	case Exit_MazeGame:
		puts("程序已退出！\n感谢您的使用！");
		break;
	case Maze_Non_Solution:
		puts("该迷宫无解");
		break;
	case Table:
		system("cls");
		puts("*************************Maze*************************");
		puts("                       游戏目录");
		puts("                   1.设定迷宫大小");
		puts("                   2.自动生成迷宫");
		puts("                   3.修改遍历顺序");
		puts("                   4.动态显示路径");
		puts("                   5.输出所有路径");
		puts("                   6.输出最短路径");
		puts("                   7.退出迷宫程序");
		puts("******************************************************");
		printf("请输入您的选择:");
		break;
	}
	ch = getch();
	return ch;	//返回选择的目录选项
}

void Print_RandomMaze()	//在屏幕上打印迷宫
{
	puts("随机生成的迷宫为：");
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			(Maze[i][j] == Wall) ? (printf("■")) : (printf("□"));
		}
		puts("");
	}
}

void Random_CreateMaze()	//随机生成迷宫
{
	srand(time(NULL));					//更新rand()函数的种子
	memset(vis, 0, sizeof(vis));		//初始化vis数组
	for (int i = 1; i <= M - 2; i++)	//按照墙路比例生成迷宫
	{
		for (int j = 1; j <= N - 2; j++)
		{
			(rand() % 10 >= P) ? (Maze[i][j] = Wall) : (Maze[i][j] = Way);
		}
	}
	for (int i = 1; i <= M - 2; i++)
	{
		for (int j = 1; j <= N - 2; j++)
		{
			if (Surround_All_Walls)	//如果一条路四周都是墙，那么全部打通
			{
				Maze[i + 1][j] = Maze[i - 1][j] = Maze[i][j + 1] = Maze[i][j - 1] = Way;
			}
			if (Surround_All_Ways)	//如果一条路四周都是路，把这条路堵死，防止路径过多
			{
				Maze[i][j] = Wall;
			}
		}
	}
	for (int i = 0; i <= M - 1; i++)	//四周封墙
	{
		for (int j = 0; j <= N - 1; j++)
		{
			if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
			{
				Maze[i][j] = Wall;
			}
		}
	}
	Maze[1][0] = Maze[1][1] = Maze[M - 2][N - 1] = Maze[M - 2][N - 2] = Way;	//起点右终点左打通
}

Status Judge(PosType e)	//判断是否可走
{
	if (e.x >= 0 && e.x < M && e.y >= 0 && e.y < N && Maze[e.x][e.y] == Way && vis[e.x][e.y] == None)	//在迷宫范围内且有路并且没有走过
	{
		return OK;
	}
	return OVERFLOW;
}

PosType NextPos(PosType e, int dir)	//下一步
{
	PosType E;
	switch (dirr[dir])
	{
	case Right:
		E.x = e.x;
		E.y = e.y + 1;
		break; //向右
	case Down:
		E.x = e.x + 1;
		E.y = e.y;
		break; //向下
	case Left:
		E.x = e.x;
		E.y = e.y - 1;
		break; //向左
	case Up:
		E.x = e.x - 1;
		E.y = e.y; //向上
	}
	return E;
}

void PrintPath(SqStack* s)	//打印路径，由于入栈时是先入后出，但是路径打印要先入先出按照从入口到出口打印，所以要用一个中间栈来改变输出顺序
{
	int IsFirst = TRUE;
	SqStack TempStack;
	InitStack(&TempStack);
	SElemType TempValue;
	while (!StackEmpty((*s)))	//当s不为空栈时，把栈s中的栈顶元素存放进TempStack栈中
	{
		Pop(s, &TempValue);
		Push(&TempStack, TempValue);
	}
	while (!StackEmpty((TempStack)))	//当TempStack不为空栈时，将TempStack中的栈顶元素放进s栈中
	{
		Pop(&TempStack, &TempValue);
		Push(s, TempValue);
		(IsFirst) ? (IsFirst = FALSE) : (printf("->"));	//如果是第一个格子就不输出箭头
		printf("(%d,%d)", TempValue.seat.x, TempValue.seat.y);	//打印路径上的每个格子的坐标
	}
	puts("");
}

void PrintMaze()	//打印迷宫
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (Maze[i][j] == Way && vis[i][j] > None)	//如果是路并且有路径
			{
				switch (dirr[vis[i][j]])	//判断方向
				{
				case Right:
					cout_green();
					printf("→");
					recover();
					break;
				case Down:
					cout_green();
					printf("↓");
					recover();
					break;
				case Left:
					cout_green();
					printf("←");
					recover();
					break;
				case Up:
					cout_green();
					printf("↑");
					recover();
					break;
				}
			}
			else if (vis[i][j] == FAIL)	//不通的路
			{
				cout_red();
				printf("×");
				recover();
			}
			else	//最后打印迷宫未走的路或者迷宫的墙
			{
				(Maze[i][j] == Wall) ? (printf("■")) : (printf("□"));
			}
		}
		puts("");
	}
}

void DFS(PosType start, PosType end, SqStack* s, int sta)	//sta=0时,检测是否可通；sta=1时，动态显示路径
{
	if (count_route == 1)
	{
		return;
	}
	if (start.x == end.x && start.y == end.y)	//走到终点
	{
		vis[end.x][end.y] = END;
		count_route = 1;
		if (sta == Dynamic)
		{
			gotoxy(0, 0);
			PrintMaze();
			puts("找到路径:");
			PrintPath(s);
			puts("");
		}
		return;
	}
	for (int i = 1; i <= 4; i++)	//遍历四个方向
	{
		if (count_route == 1)
		{
			return;
		}
		SElemType nextpos;
		nextpos.seat = NextPos(start, i);	//记录下一步的坐标
		vis[start.x][start.y] = i;	//记录下一步的方向
		if (sta == Dynamic)
		{
			gotoxy(0, 0);
			PrintMaze();
			Sleep(150);
		}
		if (!Judge(nextpos.seat))	//如果不能走的话就进入，并且如果遍历四个方向之后走不通，则该路径点不会入栈
		{
			if (i == 4)	//四个方向都走过了
			{
				vis[start.x][start.y] = FAIL;	//标记这个格子走不了
				if (sta == Dynamic)
				{
					gotoxy(0, 0);
					PrintMaze();
				}
				return;
			}
			continue;	//如果还没有四个方向走遍则换个方向走
		}
		vis[nextpos.seat.x][nextpos.seat.y] = PASS;	//标记该路径点可以通过
		Push(s, nextpos);	//将下一步可以通过的路径存入栈中
		DFS(nextpos.seat, end, s, sta);	//将下一步作为起点继续遍历
		if (vis[nextpos.seat.x][nextpos.seat.y] != FAIL)
		{
			vis[nextpos.seat.x][nextpos.seat.y] = None;
		}
		if (vis[nextpos.seat.x][nextpos.seat.y] == None)
		{
			vis[nextpos.seat.x][nextpos.seat.y] = FAIL;
		}
		Pop(s, &nextpos);
	}
}

void MulDfs(PosType start, PosType end, SqStack* s, int len)	//深度搜索所有路径
{
	if (count_route >= maxread)
	{
		return;
	}
	if (start.x == end.x && start.y == end.y)	//判断是否走通
	{
		vis[end.x][end.y] = END;
		PrintMaze();
		printf("第%d条路径已找到！该路径长度为：%d\n", ++count_route, len);
		PrintPath(s);
		ch = getch();
		puts("");
		return;
	}
	for (int i = 1; i <= 4; i++)	//如果没有走通则换个方向
	{
		SElemType nextpos;
		nextpos.seat = NextPos(start, i);
		if (!Judge(nextpos.seat))	//不能走则继续循环直到四个方向都遍历完
		{
			continue;
		}
		vis[start.x][start.y] = i;	//存储方向
		vis[nextpos.seat.x][nextpos.seat.y] = PASS;	//标记可以通过
		Push(s, nextpos);
		MulDfs(nextpos.seat, end, s, len + 1);
		vis[nextpos.seat.x][nextpos.seat.y] = None; //标记已经走过
		Pop(s, &nextpos);
	}
}

void BestDfs(PosType start, PosType end, SqStack* s, int len)	//深度搜索最短路径
{
	if (start.x == end.x && start.y == end.y)	//判断是否走通
	{
		if (len == minlength)	//输出剩余的最短路径
		{
			vis[end.x][end.y] = END;
			count_route++;
			if (count_route <= maxread)
			{
				PrintMaze();
				printf("第%d条路径已找到！该路径长度为：%d\n", count_route, len);
				PrintPath(s);
				puts("");
			}
			return;
		}
		else if (len < minlength)	//如果走通了并且路径长度比预先设定的最小长度短
		{
			minlength = len;	//重新设定最小路径长度
			system("cls");
			if (maxread != 0)
			{
				Print_RandomMaze();
				puts("");
				PrintMaze();
				printf("第1条路径已找到！\n");
				PrintPath(s);
				puts("");
			}
			count_route = 1;
		}
		return;
	}
	for (int i = 1; i <= 4; i++)	//如果没有走通则换个方向
	{
		SElemType nextpos;
		nextpos.seat = NextPos(start, i);
		if (!Judge(nextpos.seat))	//不能走则继续循环直到四个方向都遍历完
		{
			continue;
		}
		vis[start.x][start.y] = i;	//存储下一个位置的方向
		vis[nextpos.seat.x][nextpos.seat.y] = PASS;	//标记可以通过
		Push(s, nextpos);
		BestDfs(nextpos.seat, end, s, len + 1);
		vis[nextpos.seat.x][nextpos.seat.y] = 0;
		Pop(s, &nextpos);
	}
}

void Get_MazeSize()	//获取迷宫的长度和宽度
{
	while (1)
	{
		system("cls");
		fflush(stdin);
		printf("请输入迷宫的行数[%d,%d]：", Min_Row, Max_Row);
		if (scanf("%d", &M) == ERROR || M > Max_Row || M < Min_Row)
		{
			MenuTable(Input_ERROR);
			continue;
		}
		break;
	}
	while (1)
	{
		system("cls");
		fflush(stdin);
		printf("请输入迷宫的列数[%d,%d]：", Min_Column, Max_Column);
		if (scanf("%d", &N) == ERROR || N > Max_Column || N < Min_Column)
		{
			MenuTable(Input_ERROR);
			continue;
		}
		break;
	}
}

void Get_Route_Numb()	//输入查看路径的数量
{
	while (1)
	{
		fflush(stdin);
		puts("请输入您要查看路径的数量上限[1,1500]");
		if (!scanf("%d", &maxread) || maxread < 1 || maxread > 1500)
		{
			MenuTable(Input_ERROR);
			continue;
		}
		fflush(stdin);
		break;
	}
}

void Get_Maze_Solution(PosType start, PosType end, SqStack S)	//判断是否生成无解的迷宫
{
	while (1)
	{
		system("cls");
		fflush(stdin);
		puts("是否生成无解的迷宫：\n1.是\n2.否");
		switch (char solution = getch())
		{
		case '1':
			Random_CreateMaze();
			DFS(start, end, &S, 0);
			if (count_route == 0)
			{
				puts("迷宫生成成功且该迷宫没有解！");
				non_solution = Maze_Non_Solution;
			}
			else
			{
				puts("迷宫生成成功且该迷宫有解！");
				non_solution = FALSE;
			}
			break;
		case '2':
			do
			{
				Random_CreateMaze();
				DFS(start, end, &S, 0);
			} while (count_route == 0);	//生成迷宫直到有解
			break;
		default:
			MenuTable(Input_ERROR);
			continue;
		}
		break;
	}
	memset(vis, 0, sizeof(vis));	//重置vis数组
	Print_RandomMaze();
	count_route = 0;
	MenuTable(Back_Menu);
}

void Reinitialize_Variable(SqStack S)	//重新初始化路径数组、count_route、栈以及返回菜单
{
	count_route = 0;	//重新初始化
	StackClear(&S);
	memset(vis, 0, sizeof(vis));
	MenuTable(Back_Menu);
}

void Change_Order()	//改变遍历的方向
{
	while (1)
	{
		system("cls");
		puts("请输入遍历顺序对应的编号：1.右 2.下 3.左 4.上");
		for (int i = 1; i <= 4; i++)
		{
			printf("请输入第%d个：", i);
			fflush(stdin);
			scanf("%d", &dirr[i]);
		}
		if (Judge_Input_Direction)
		{
			fflush(stdin);
			break;
		}
		MenuTable(Input_ERROR);
		continue;
	}
	puts("遍历顺序修改成功！");
	MenuTable(Back_Menu);
}

// ===== 只有一条路径的函数原型及主函数 =====

	// ----- 主要函数 -----

int OneRoute()
{
	while (true)
	{
		system("cls");
		char choice;
		printf("\n\n*****\t欢迎来到单线迷宫求解问题\t******\n\n\n");
		printf("\t[1] 随机生成迷宫.\n");
		printf("\t[2] 退出游戏.\n\n");
		printf("\t\t随机生成 X*Y 的迷宫\n\n");
		printf("********* 走出迷宫 走向成功 ***********\n\n");
		printf("请输入你的选择：");
		scanf("%c", &choice);
		if (choice == '1')
		{
			printf("\n请输入X[5,50]和Y[5,50]的值(空格隔开): ");
			int num = scanf("%d %d", &X, &Y);
			if (!num || X < 5 || X>50 || Y < 5 || Y>50)
			{
				printf("请输入正确的X和Y\n");
				while (char ch = getchar() != '\n');
				system("pause");
				continue;
			}
			printf("\n正在随机生成一个%d×%d的迷宫\n", X, Y);
			X = X + 2;
			Y = Y + 2;
			int* mg = (int*)malloc(sizeof(int) * X * Y);
			int* mglx = (int*)malloc(sizeof(int) * X * Y);
			system("cls");
		create:while (true)
		{
			create_mg(mg, mglx, X, Y);
			if (*(mg + X * Y - 2 * Y - 3) == 0)	//若终点是一堵墙，则重新生成迷宫
			{
				break;
			}
		}
		copy_mg(mg, mglx, X, Y);
		show_mg(mg, mglx, X, Y);
		int show;
		printf("输入1展示路径\n输入其他重新生成迷宫\n");
		scanf("%d", &show);
		getchar();
		if (show == 1)
		{
			play_mg(mg, mglx, X, Y);
			show_Route(mg, mglx, X, Y);
		}
		else
		{
			system("cls");
			goto create;
		}
		}
		else if (choice == '2')
		{
			return 0;
		}
		else
		{
			printf("请输入正确的选择!\n");
		}
		while (char ch = getchar() != '\n');
		system("pause");
		continue;
	}
}

	// ----- 函数原型定义 -----

void create_mg(int mg[], int mglx[], int X, int Y)//随机生成迷宫
{
	//初始化迷宫：内部为墙体；外围为通路，当做屏障
	for (int i = 0; i < Y; i++)
	{
		mg[0 * Y + i] = 0;
		mg[(X - 1) * Y + i] = 0;
	}
	for (int i = 1; i < X - 1; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			(j == 0 || j == Y - 1) ? (mg[i * Y + j] = 0) : (mg[i * Y + j] = 1);
		}
	}

	//分别对应着上下左右方向
	int dRow[] = { 1, 0, -1, 0 };
	int dCol[] = { 0, -1, 0, 1 };
	//用于存储墙体坐标
	vector<int> QX, QY;
	QX.push_back(start_x);//从起点开始
	QY.push_back(start_y);
	while (QX.size())//拆墙
	{
		//根据当前时间，生成一个随机数种子
		srand((unsigned int)(time(NULL)));
		int index = rand() % (QX.size());//随机生成一个坐标，选取一堵墙
		int x = QX[index];
		int y = QY[index];
		int flag = 0;//记录该墙四周通路个数
		int r, c;
		for (int i = 0; i < 4; i++)
		{
			r = x + dRow[i];
			c = y + dCol[i];
			if (mg[r * Y + c] == 0)
			{
				flag++;
			}
		}

		if (flag <= 1)//当该墙体四周通路有一条或没有时，则该墙改为道路
		{
			mg[x * Y + y] = 0;
			for (int i = 0; i < 4; i++)
			{
				r = x + dRow[i];
				c = y + dCol[i];
				if (mg[r * Y + c] == 1)//将该墙四周的墙加入队列
				{
					QX.push_back(r);
					QY.push_back(c);
				}
			}
		}
		//将当前墙移除队列
		QX.erase(QX.begin() + index);
		QY.erase(QY.begin() + index);
	}
}

void play_mg(int mg[], int mglx[], int X, int Y)
{
	//分别对应着上下左右方向
	int dRow[] = { 0, 1, 0, -1 };
	int dCol[] = { 1, 0, -1, 0 };

	SElemType start;//起始位置
	start.seat.x = start_x;
	start.seat.y = start_y;
	Push(&Route, start);
	while (!StackEmpty(Route))
	{
		SElemType temp;
		temp.di = 0;
		temp.seat.x = 0;
		temp.seat.y = 0;
		bool flag = false;//判断该坐标是否有通路
		SElemType top;
		GetTop(&Route, &top);
		int x = top.seat.x;
		int y = top.seat.y;
		mg[x * Y + y] = 1;//该坐标已经过,设置为墙
		for (int i = 0; i < 4; i++) //向四个位置进行试探,方向：右1 下2 左3 上4
		{
			int r = x + dRow[i];
			int c = y + dCol[i];
			if (mg[r * Y + c] == 0)
			{
				temp.seat.x = r;
				temp.seat.y = c;
				temp.di = i + 1;
				Push(&Route, temp);
				flag = true;
				break;
			}
		}
		if (x == X - 3 && y == Y - 3)//到达终点
		{
			break;
		}
		mglx[x * Y + y] = temp.di + 1;//该坐标已经过,右2 下3 左4 上5
		if (!flag)//若该坐标点无通路，则回溯
		{
			SElemType t;
			GetTop(&Route, &t);
			mglx[t.seat.x * Y + t.seat.y] = 6;//回溯后要把该坐标设为不可走路线
			SElemType pope;
			Pop(&Route, &pope);
		}
		HideCursor();
		gotoxy(0, 0);
		show_mg(mg, mglx, X, Y);
	}
}

void show_Route(int mg[], int mglx[], int X, int Y)//展示坐标路线
{
	SElemType temp[100];
	int length = 0;
	while (!StackEmpty(Route))
	{
		GetTop(&Route, &temp[length]);
		length++;
		SElemType pope;
		Pop(&Route, &pope);
	}
	printf("\n\n方向路线:\n");
	printf("起点=>");
	for (int i = length - 1; i >= 0; i--)
	{
		if (temp[i].di == 1)
		{
			printf("右=>");
		}
		else if (temp[i].di == 2)
		{
			printf("下=>");
		}
		else if (temp[i].di == 3)
		{
			printf("左=>");
		}
		else if (temp[i].di == 4)
		{
			printf("上=>");
		}
	}
	printf("终点\n\n");
	printf("坐标路线：\n");
	printf("起点=>");
	for (int i = length - 1; i >= 0; i--)
	{
		printf("(%d,%d)=>", temp[i].seat.x - 1, temp[i].seat.y - 1);
	}
	printf("终点\n");
}

void show_mg(int mg[], int mglx[], int X, int Y)//展示迷宫路线
{
	for (int i = 1; i < X - 1; i++)
	{
		for (int j = 1; j < Y - 1; j++)
		{
			//墙■路□右2→ 下3↓ 左4← 上5↑ 错×
			if (i == start_x && j == start_y)
				printf("☆");
			else if (i == X - 3 && j == Y - 3)
			{
				printf("★");
			}
			else if (mglx[i * Y + j] == 1)
			{
				printf("■");
			}
			else if (mglx[i * Y + j] == 2)
			{
				cout_green();
				printf("→");
				recover();
			}
			else if (mglx[i * Y + j] == 3)
			{
				cout_green();
				printf("↓");
				recover();
			}
			else if (mglx[i * Y + j] == 4)
			{
				cout_green();
				printf("←");
				recover();
			}
			else if (mglx[i * Y + j] == 5)
			{
				cout_green();
				printf("↑");
				recover();
			}
			else if (mglx[i * Y + j] == 6)
			{
				cout_red();
				printf("×");
				recover();
			}
			else
			{
				printf("□");
			}
		}
		printf("\n");
	}
}

void copy_mg(int mg[], int mglx[], int X, int Y)//复制迷宫
{
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			mglx[i * Y + j] = mg[i * Y + j];
		}
	}
}