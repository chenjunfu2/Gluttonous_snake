#pragma once
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#include <Windows.h>


//标记
#define FOOD_MARK -1//食物标记
#define SNAKE_HEAD 1//蛇头标记
#define NULL_BLOCK 0//空地标记

struct My_Point
{
	long x, y;

public:
	My_Point &operator+=(const My_Point &_Other)
	{
		x += _Other.x;
		y += _Other.y;
		return *this;
	}
};


class Snake_Data
{
public:
	enum Move_Direct :long
	{
		Up = 0,
		Down,
		Left,
		Right,
		Arr_Size,
		No_Move = Arr_Size,
	};

private:
	const long lMapWidth;//x
	const long lMapHigh;//y
	long *lMap;//地图

	long lSnakeLength;//蛇长度
	My_Point stSnakeHead;//头坐标
	Move_Direct enMoveDirect;//移动方向
public:

	Snake_Data(long _lMapWidth, long _lMapHigh, const My_Point &_stSnakeStart, Move_Direct _enDirectStart) :
		lMapWidth(_lMapWidth), lMapHigh(_lMapHigh)
	{
		lMap = new long[lMapWidth * lMapHigh];
		Reset(_stSnakeStart, _enDirectStart);
	}

	~Snake_Data(void)
	{
		delete[] lMap;
		lMap = NULL;
	}

	void Reset(const My_Point &_stSnakeStart, Move_Direct _enDirectStart)
	{
		stSnakeHead = _stSnakeStart;
		enMoveDirect = _enDirectStart;
		lSnakeLength = 1;//蛇长度设为1

		memset(lMap, 0, lMapWidth * lMapHigh * sizeof(long));
		GetMap(stSnakeHead) = SNAKE_HEAD;//初始化地图中蛇头的位置
	}

	long &GetSnakeLength(void)
	{
		return lSnakeLength;
	}

	const long &GetSnakeLength(void) const
	{
		return lSnakeLength;
	}

	My_Point &GetSnakeHead(void)
	{
		return stSnakeHead;
	}

	const My_Point &GetSnakeHead(void) const
	{
		return stSnakeHead;
	}

	Move_Direct &GetMoveDirect(void)
	{
		return enMoveDirect;
	}

	const Move_Direct &GetMoveDirect(void) const
	{
		return enMoveDirect;
	}

	long &GetMap(const My_Point &stPoint)
	{
		return lMap[stPoint.y * lMapWidth + stPoint.x];
	}

	const long &GetMap(const My_Point &stPoint) const
	{
		return lMap[stPoint.y * lMapWidth + stPoint.x];
	}

	long GetMapWidth(void) const
	{
		return lMapWidth;
	}

	long GetMapHigh(void) const
	{
		return lMapHigh;
	}
};

class Control
{
private:
	static bool Cross(const Snake_Data &csSnakeData, const My_Point &stNewSnakeHead)
	{
		//越界
		if (stNewSnakeHead.x >= csSnakeData.GetMapWidth() ||
			stNewSnakeHead.y >= csSnakeData.GetMapHigh() ||
			stNewSnakeHead.x < 0 ||
			stNewSnakeHead.y < 0)
		{
			return true;
		}
		return false;
	}

	static constexpr My_Point stSnakeMove[(long)Snake_Data::Arr_Size] =//移动方向
	{
		{ 0,-1},
		{ 0, 1},
		{-1, 0},
		{ 1, 0},
	};
public:
	static void Wait(long lMillisecond)
	{
		Sleep(lMillisecond);
	}

	static bool Win(const Snake_Data &csSnakeData)
	{
		if (csSnakeData.GetSnakeLength() == csSnakeData.GetMapHigh() * csSnakeData.GetMapWidth())
		{
			return true;
		}
		return false;
	}

	static bool Lose(const Snake_Data &csSnakeData, const My_Point &stNewSnakeHead)
	{
		//越界或碰到蛇身
		if (Cross(csSnakeData, stNewSnakeHead) || csSnakeData.GetMap(stNewSnakeHead) > NULL_BLOCK)
		{
			return true;
		}

		return false;
	}

	static Snake_Data::Move_Direct Input(void)
	{
		while (_kbhit() != 0)//存在一个输入
		{
			switch (_getch())
			{
			case 'w':
			case 'W':
				{
					return Snake_Data::Up;
				}
				break;
			case 'a':
			case 'A':
				{
					return Snake_Data::Left;
				}
				break;
			case 's':
			case 'S':
				{
					return Snake_Data::Down;
				}
				break;
			case 'd':
			case 'D':
				{
					return Snake_Data::Right;
				}
				break;
			case 'p'://暂停
			case 'P':
				{
					int iGet;
					do
					{
						iGet = _getch();
					} while (iGet != 'p' && iGet != 'P');
				}
				continue;
			default:
				continue;
			}
		}

		return Snake_Data::No_Move;
	}

	static bool SwitchInput(const char *cWaitInInputTrue, long lTrueConut, const char *cWaitInInputFalse, long lFalseConut)
	{
		char ch;
		while (true)
		{
			ch = _getch();
			for (long i = 0; i < lTrueConut; ++i)
			{
				if (ch == cWaitInInputTrue[i])
				{
					return true;
				}
			}
			for (long i = 0; i < lFalseConut; ++i)
			{
				if (ch == cWaitInInputFalse[i])
				{
					return false;
				}
			}
		}
	}

	static void GetAnyKey(void)
	{
		(void)_getch();
	}

	static void ChangeDirect(Snake_Data &csSnakeData, Snake_Data::Move_Direct enMoveDirect)
	{
		//判断有没有进行移动
		if (enMoveDirect != Snake_Data::No_Move)
		{
			//判断改变的方向是否是蛇身
			My_Point stCurrent = csSnakeData.GetSnakeHead();
			stCurrent += stSnakeMove[(long)enMoveDirect];
			if (csSnakeData.GetMap(stCurrent) != SNAKE_HEAD + 1)//不是蛇身
			{
				csSnakeData.GetMoveDirect() = enMoveDirect;//改变方向
			}
		}
	}

	static void ProduceFood(Snake_Data &csSnakeData)
	{
		if (csSnakeData.GetSnakeLength() >= csSnakeData.GetMapHigh() * csSnakeData.GetMapWidth())
		{
			return;//生成失败
		}

		My_Point stFood;
		do
		{
			stFood.x = rand() % csSnakeData.GetMapWidth();
			stFood.y = rand() % csSnakeData.GetMapHigh();
		} while (csSnakeData.GetMap(stFood) != 0);//遇到空地为止

		csSnakeData.GetMap(stFood) = FOOD_MARK;
	}

	static bool Move(Snake_Data &csSnakeData)
	{
		//先将蛇头向前移动一格
		My_Point stNewSnakeHead = csSnakeData.GetSnakeHead();
		stNewSnakeHead += stSnakeMove[(long)csSnakeData.GetMoveDirect()];

		//判断是否输了
		if (Lose(csSnakeData, stNewSnakeHead))
		{
			return false;
		}

		//设置蛇头位置
		csSnakeData.GetSnakeHead() = stNewSnakeHead;

		//判断是否吃到食物
		bool bEatFood = false;
		if (csSnakeData.GetMap(stNewSnakeHead) == FOOD_MARK)
		{
			++csSnakeData.GetSnakeLength();//增加蛇的长度
			ProduceFood(csSnakeData);//生成新的食物
			bEatFood = true;
		}

		//更新新蛇头
		csSnakeData.GetMap(stNewSnakeHead) = SNAKE_HEAD;

		//更新蛇身
		My_Point stCurrent = stNewSnakeHead;//后续此值为蛇尾坐标
		for (long i = 0; i < csSnakeData.GetSnakeLength(); ++i)
		{
			//查看四个方向上与自身值相同的值
			for (long j = 0; j < (long)Snake_Data::Arr_Size; ++j)
			{
				My_Point stMove = stCurrent;
				stMove += stSnakeMove[j];//移动

				if (!Cross(csSnakeData, stMove) && csSnakeData.GetMap(stMove) == csSnakeData.GetMap(stCurrent))//相同
				{
					++csSnakeData.GetMap(stMove);//递增
					stCurrent = stMove;//移动
				}
			}
		}

		//更新蛇尾
		if (!bEatFood)
		{
			csSnakeData.GetMap(stCurrent) = NULL_BLOCK;//设置蛇尾为空
		}

		return true;
	}
};

class Draw
{
private:
	//绘制
	static void DrawBorder(void)//绘制边界
	{
		fputs("■", stdout);
	}

	static void DrawNull(void)//绘制空地
	{
		fputs("  ", stdout);
	}

	static void DrawFood(void)//绘制食物
	{
		fputs("◇", stdout);
	}

	static void DrawSnakeHead(void)//绘制蛇头
	{
		fputs("○", stdout);
	}

	static void DrawSnakeBody(void)//绘制蛇身
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

	static void DrawLineBorder(const Snake_Data &csSnakeData)//绘制行边界
	{
		for (long x = 0; x < csSnakeData.GetMapWidth() + 2; ++x)//+2是因为左侧和右侧各有1格边界
		{
			DrawBorder(); 
		}
		NewLine();
	}

public:
	static void InfoDraw(void)
	{
		printf("---贪吃蛇---\nW-上  A-下\nS-左  D-右\nP-暂停/继续\n------------\n\n按任意键开始...");
		Flush();
	}

	static void SnakeDraw(const Snake_Data &csSnakeData)
	{
		//隐藏光标并定位到开头
		HideCursor();
		RewindOutput();

		//绘制上边界
		DrawLineBorder(csSnakeData);

		for (long y = 0; y < csSnakeData.GetMapHigh(); ++y)
		{
			DrawBorder();//绘制左边界
			for (long x = 0; x < csSnakeData.GetMapWidth(); ++x)
			{
				if (csSnakeData.GetMap({x,y}) == NULL_BLOCK)
				{
					DrawNull();
				}
				else if (csSnakeData.GetMap({x,y}) == SNAKE_HEAD)
				{
					DrawSnakeHead();
				}
				else if (csSnakeData.GetMap({x,y}) == FOOD_MARK)
				{
					DrawFood();
				}
				else//蛇身
				{
					DrawSnakeBody();
				}
			}
			DrawBorder();//绘制右边界
			NewLine();//到下一行绘制
		}

		//绘制下边界
		DrawLineBorder(csSnakeData);

		printf("长度:%ld\n", csSnakeData.GetSnakeLength());

		//刷新
		Flush();
	}

	static void LoseDraw(void)
	{
		printf("\n你输了，");
		Flush();
	}

	static void WinDraw(void)
	{
		printf("\n你赢了，");
		Flush();
	}

	static void EndGame(const char *pNew, const char *pLeave)
	{
		printf("输入%s新开一局，输入%s结束游戏", pNew, pLeave);
		Flush();
	}

	static void Clear(void)//清屏
	{
		system("cls");
	}
};




class Game
{
private:
	Snake_Data &csSnakeData;
	long lLoopSpeed;
public:
	Game(Snake_Data &_csSnakeData, long _lLoopSpeed) :
		csSnakeData(_csSnakeData), lLoopSpeed(_lLoopSpeed)
	{
		//设置缓冲区为全缓冲，大小为地图大小
		setvbuf(stdout, NULL, _IOFBF, csSnakeData.GetMapHigh() * csSnakeData.GetMapWidth() + 1);
		//初始化随机数发生器
		srand((unsigned int)time(NULL));
		//输出开始信息
		Draw::InfoDraw();
		//按任意键继续
		Control::GetAnyKey();
	}

	void Init(void)
	{
		//初始化食物
		Control::ProduceFood(csSnakeData);
	}

	void ChangeLoopSpeed(long lNewLoopSpeed)
	{
		lLoopSpeed = lNewLoopSpeed;
	}

	long GetLoopSpeed(void) const
	{
		return lLoopSpeed;
	}

	bool Loop(void)
	{
		while (true)
		{
			Control::Wait(lLoopSpeed);
			Control::ChangeDirect(csSnakeData, Control::Input());

			if (!Control::Move(csSnakeData))
			{
				Draw::SnakeDraw(csSnakeData);
				Draw::LoseDraw();
				break;
			}

			Draw::SnakeDraw(csSnakeData);

			if (Control::Win(csSnakeData))
			{
				Draw::WinDraw();
				break;
			}
		}

		Draw::EndGame("H", "L");
		return Control::SwitchInput("Hh", 2, "Ll", 2);
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