#pragma once

#include "Game_Data.hpp"

#include <conio.h>
#include <Windows.h>
#include <random>

class Control
{
private:
	static bool Cross(const Game_Data &csGameData, const My_Point &stNewSnakeHead)
	{
		//越界
		if (stNewSnakeHead.x >= csGameData.GetMapWidth() ||
			stNewSnakeHead.y >= csGameData.GetMapHigh() ||
			stNewSnakeHead.x < 0 ||
			stNewSnakeHead.y < 0)
		{
			return true;
		}
		return false;
	}

	static constexpr My_Point stSnakeMove[(long)Game_Data::Move_Direct::Arr_End] =//移动方向
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

	static bool Win(const Game_Data &csGameData)
	{
		if (csGameData.GetSnakeLength() == csGameData.GetMapHigh() * csGameData.GetMapWidth())
		{
			return true;
		}
		return false;
	}

	static Game_Data::Move_Direct Input(void)
	{
		while (_kbhit() != 0)//存在一个输入
		{
			switch (_getch())
			{
			case 'w':
			case 'W':
				{
					return Game_Data::Move_Direct::Up;
				}
				break;
			case 'a':
			case 'A':
				{
					return Game_Data::Move_Direct::Left;
				}
				break;
			case 's':
			case 'S':
				{
					return Game_Data::Move_Direct::Down;
				}
				break;
			case 'd':
			case 'D':
				{
					return Game_Data::Move_Direct::Right;
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
					Wait(50);//给50ms反应时间
				}
				continue;
			default:
				continue;
			}
		}

		return Game_Data::Move_Direct::No_Move;
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

	static void SetOutputFullMode(const Game_Data &csGameData)
	{
		//设置缓冲区为全缓冲，大小为地图大小
		setvbuf(stdout, NULL, _IOFBF, (csGameData.GetMapHigh() + 2) * (csGameData.GetMapWidth() + 2) + 1);
	}

	static void SetOutputLineMode(void)
	{
		setvbuf(stdout, NULL, _IOLBF, BUFSIZ);//恢复默认
	}

	static void ChangeDirect(Game_Data &csGameData, Game_Data::Move_Direct enMoveDirect)
	{
		//判断有没有进行移动
		if (enMoveDirect != Game_Data::Move_Direct::No_Move)
		{
			//判断改变的方向是否是蛇身
			My_Point stCurrent = csGameData.GetSnakeHead();
			stCurrent += stSnakeMove[(long)enMoveDirect];
			if (csGameData.GetMap(stCurrent) == 0 || csGameData.GetMap(stCurrent) + 1 != csGameData.GetSnakeLength())//不是蛇身
			{
				csGameData.GetMoveDirect() = enMoveDirect;//改变方向
			}
		}
	}

	static void ProduceFood(Game_Data &csGameData, std::mt19937 csRandom)//后续修改生成器
	{
		if (csGameData.GetSnakeLength() >= csGameData.GetMapHigh() * csGameData.GetMapWidth())
		{
			return;//生成失败
		}

		My_Point stFood;
		std::uniform_int_distribution<long> csDistX(0, csGameData.GetMapWidth() - 1);//max能取到
		std::uniform_int_distribution<long> csDistY(0, csGameData.GetMapHigh() - 1);//所以要-1
		do
		{
			stFood.x = csDistX(csRandom);
			stFood.y = csDistY(csRandom);
		} while (csGameData.GetMap(stFood) != 0);//遇到空地为止

		csGameData.GetMap(stFood) = FOOD_BLOCK;
	}

	static bool Move(Game_Data &csGameData, std::mt19937 &csRandom)
	{
		//先将蛇头向当前方向移动一格
		My_Point stNewSnakeHead = csGameData.GetSnakeHead();
		stNewSnakeHead += stSnakeMove[(long)csGameData.GetMoveDirect()];

		//判断是否输了
		if (Cross(csGameData, stNewSnakeHead) || csGameData.GetMap(stNewSnakeHead) > NULL_BLOCK)//撞墙或吃到自身
		{
			return false;
		}

		//判断是否吃到食物
		if (csGameData.GetMap(stNewSnakeHead) == FOOD_BLOCK)
		{
			//更新新蛇头为原先蛇头的值+1，并不移动蛇尾
			csGameData.GetMap(stNewSnakeHead) = csGameData.GetSnakeLength() + 1;
			//设置新蛇头位置
			csGameData.GetSnakeHead() = stNewSnakeHead;
			//生成新的食物
			ProduceFood(csGameData, csRandom);

			return true;
		}

		//更新新蛇头为原先蛇头的值
		csGameData.GetMap(stNewSnakeHead) = csGameData.GetSnakeLength();
		//设置新蛇头位置
		csGameData.GetSnakeHead() = stNewSnakeHead;

		//更新蛇身与蛇尾
		My_Point stCurrent = stNewSnakeHead;//后续此值为蛇尾坐标
		while (csGameData.GetMap(stCurrent) != NULL_BLOCK)
		{
			//查看四个方向上与自身值相同的值
			for (long j = (long)Game_Data::Move_Direct::Arr_Beg; j < (long)Game_Data::Move_Direct::Arr_End; ++j)
			{
				My_Point stMove = stCurrent;
				stMove += stSnakeMove[j];//移动

				if (!Cross(csGameData, stMove) && csGameData.GetMap(stMove) == csGameData.GetMap(stCurrent))//相同
				{
					--csGameData.GetMap(stMove);//递减
					stCurrent = stMove;//移动
					break;//处理下一个位置
				}
			}
		}

		return true;
	}
};