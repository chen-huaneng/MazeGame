#include <windows.h>	//颜色改变，时停
#include<stdlib.h>	//清屏

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);//颜色

//清屏函数，代替system("cls")，防止闪烁，网上学习
void gotoxy(int x, int y);	//将光标移动到（x,y）位置
void HideCursor();//隐藏光标
void cout_green();	//打印绿色
void cout_red();	//打印红色
void recover();	//恢复白色

void cout_green()//绿色
{
	SetConsoleTextAttribute(hout, 2);
}
void cout_red()//红色
{
	SetConsoleTextAttribute(hout, 4);
}
void recover()//白色
{
	SetConsoleTextAttribute(hout, 1 | 2 | 4);
}
void gotoxy(int x, int y)//将光标移动到（x,y）位置
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    //获取标准输出设备句柄
	COORD pos = { x,y };     //坐标位置
	SetConsoleCursorPosition(handle, pos);   //设置控制台光标位置
}
void HideCursor()//隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 }; //0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}