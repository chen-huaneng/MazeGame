#include "MazeGameFunction.h"
#include <stdio.h>

int main()
{
	system("cls");
	puts("***********************Maze***********************");
	puts("\t\t1:随机生成迷宫");
	puts("\t\t2:一条路径迷宫");
	puts("\t\t其他:退出程序");
	puts("**************************************************");
	printf("请输入您的选择:");
	char ch;
	switch (ch = getchar())
	{
	case '1':
		MazeGame_Random();
		break;
	case '2':
		OneRoute();
		break;
	default:
		return 0;
	}
}