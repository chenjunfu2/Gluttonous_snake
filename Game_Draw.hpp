#pragma once

#include "Game_Data.hpp"
#include "Game_Sign.hpp"

#include <stdio.h>
#include <Windows.h>

class Game_Draw
{
private:
	//绘制
	static void BorderBlock(void)//绘制边界
	{
		fputs("■", stdout);
	}

	static void NullBlock(void)//绘制空地
	{
		fputs("  ", stdout);
	}

	static void FoodBlock(void)//绘制食物
	{
		fputs("◇", stdout);
	}

	static void SnakeHead(void)//绘制蛇头
	{
		fputs("○", stdout);
	}

	static void SnakeBody(void)//绘制蛇身
	{
		fputs("□", stdout);
	}

	static void SnakeTail(void)//绘制蛇尾
	{
		fputs("□", stdout);
	}

	static void RewindOutput(void)//回到开头
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
	}

	static void NewLine(void)//换行
	{
		putchar('\n');
	}

	static void HideCursor(void)//隐藏光标
	{
		CONSOLE_CURSOR_INFO info = {1,0};
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}

	static void Flush(void)//刷新输出
	{
		fflush(stdout);
	}

	static void LineBorderBlock(const Game_Data &csGameData)//绘制行边界
	{
		for (long x = 0; x < csGameData.GetMapWidth() + 2; ++x)//+2是因为左侧和右侧各有1格边界
		{
			BorderBlock();
		}
		NewLine();
	}

public:
	static void Info(void)
	{
		printf("---贪吃蛇---\nW--上  A--下\nS--左  D--右\n\nP--暂停/继续\n------------\n\n按任意键开始...");
		Flush();
	}

	static void Draw(const Game_Data &csGameData)
	{
		//隐藏光标并定位到开头
		HideCursor();
		RewindOutput();

		//绘制上边界
		LineBorderBlock(csGameData);

		for (long y = 0; y < csGameData.GetMapHigh(); ++y)
		{
			BorderBlock();//绘制左边界
			for (long x = 0; x < csGameData.GetMapWidth(); ++x)
			{
				long lCurrent = csGameData.GetMap({x,y});

				if (lCurrent == NULL_BLOCK)
				{
					NullBlock();
				}
				else if (lCurrent == csGameData.GetSnakeLength())//如果蛇只有1格，头和尾是相同的，但是头部优先绘制
				{
					SnakeHead();
				}
				else if (lCurrent == SNAKE_TAIL)
				{
					SnakeTail();
				}
				else if (lCurrent == FOOD_BLOCK)
				{
					FoodBlock();
				}
				else//蛇身
				{
					SnakeBody();
				}
			}
			BorderBlock();//绘制右边界
			NewLine();//到下一行绘制
		}

		//绘制下边界
		LineBorderBlock(csGameData);

		printf("长度:%ld\n", csGameData.GetSnakeLength());

		//刷新
		Flush();
	}

	static void Lose(void)
	{
		printf("\n你输了！\n");
		Flush();
	}

	static void Win(void)
	{
		printf("\n你赢了！\n");
		Flush();
	}

	static void End(const char *pNew, const char *pLeave)
	{
		printf("输入%s新开一局\n输入%s结束游戏", pNew, pLeave);
		Flush();
	}

	static void Clear(void)//清屏
	{
		system("cls");
	}
};

/*
\033[显示方式；前景色；背景色m

ANSI控制码 :
\033[0m   关闭所有属性
\033[1m   设置高亮度
\033[4m   下划线
\033[5m   闪烁
\033[7m   反显
\033[8m   消隐
\033[30m   --   \033[37m   设置前景色
\033[40m   --   \033[47m   设置背景色
\033[nA   光标上移n行
\033[nB   光标下移n行
\033[nC   光标右移n行
\033[nD   光标左移n行
\033[y;xH 设置光标位置
\033[2J   清屏
\033[K   清除从光标到行尾的内容
\033[s   保存光标位置
\033[u   恢复光标位置
\033[?25l   隐藏光标
\033[?25h   显示光标

例：
\033[2J\033[0;0H  清屏且将光标置顶
\033[1;31mhello world\033[0m  使用高亮红色输出hello world并恢复颜色为NONE


颜色分为背景色和字体色，30~39用来设置字体色，40~49设置背景：

背景色                        字体色
40: 黑                          30: 黑
41: 红                          31: 红
42: 绿                          32: 绿
43: 黄                          33: 黄
44: 蓝                          34: 蓝
45: 紫                          35: 紫
46: 深绿                        36: 深绿
47: 白色                        37: 白色

记得在打印完之后，把颜色恢复成NONE，不然再后面的打印都会跟着变色。

其它一些转移字符
\a alarm（闹钟；警报）
\b backspace (退格键)
\f form feed（换页符）
\t Tab（Tab按键）
\v Vertical Tab（水平制表）
\n newline（换行）
\r carriage return（回车）
*/