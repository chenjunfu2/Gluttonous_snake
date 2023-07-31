#pragma once

#include "Game_Data.hpp"

#include <conio.h>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>

class Game_Control
{
public:
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

	static void Wait(long lMillisecond)
	{
		Sleep(lMillisecond);
	}

	static bool Win(const Game_Data &csGameData)
	{
		if (csGameData.GetSnakeLength() >= csGameData.GetWinLength())
		{
			return true;
		}
		return false;
	}

	static Game_Data::Move_Direct Input(void)
	{
		while (_kbhit() != 0)//存在一个输入
		{
			switch (_getwch())
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
						iGet = _getwch();
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

	static bool SwitchInput(const wchar_t *cWaitInInputTrue, long lTrueConut, const wchar_t *cWaitInInputFalse, long lFalseConut)
	{
		wchar_t wch;
		while (true)
		{
			wch = _getwch();
			for (long i = 0; i < lTrueConut; ++i)
			{
				if (wch == cWaitInInputTrue[i])
				{
					return true;
				}
			}
			for (long i = 0; i < lFalseConut; ++i)
			{
				if (wch == cWaitInInputFalse[i])
				{
					return false;
				}
			}
		}
	}

	static void GetAnyKey(void)
	{
		(void)_getwch();
	}

	static void SetIOMode(const Game_Data &csGameData)
	{
		//设置缓冲区为全缓冲，大小为地图大小
		setvbuf(stdout, NULL, _IOFBF, (csGameData.GetMapHigh() + 2) * (csGameData.GetMapWidth() + 2) + 1);
		(void)_setmode(_fileno(stdout), _O_U16TEXT);//设置输出字符集
		(void)_setmode(_fileno(stdin), _O_U16TEXT);//设置输入字符集
	}

	static void ChangeDirect(Game_Data &csGameData, Game_Data::Move_Direct enMoveDirect)
	{
		//判断有没有进行移动
		if (enMoveDirect != Game_Data::Move_Direct::No_Move)
		{
			//判断改变的方向是否是蛇身
			My_Point stCurrent = csGameData.GetSnakeHead();
			stCurrent += Game_Data::stSnakeMove[(long)enMoveDirect];
			if (csGameData.GetMap(stCurrent) == 0 || csGameData.GetMap(stCurrent) + 1 != csGameData.GetSnakeLength())//不是蛇身
			{
				csGameData.GetMoveDirect() = enMoveDirect;//改变方向
			}
		}
	}

	static void ProduceFood(Game_Data &csGameData)//后续修改生成器
	{
		long lFreeSpace = csGameData.GetMapHigh() * csGameData.GetMapWidth() - csGameData.GetSnakeLength();
		long lProduceNum = min(lFreeSpace, csGameData.GetFoodMaxNum());//获取空位最小值和生成大小最小值

		if (lProduceNum <= 0)//地图没空间了
		{
			return;//生成失败
		}

		My_Point stFood;
		std::mt19937 &csRandom = csGameData.GetRandom();
		std::uniform_int_distribution<long> csDistX(0, csGameData.GetMapWidth() - 1);//max能取到所以要-1
		std::uniform_int_distribution<long> csDistY(0, csGameData.GetMapHigh() - 1);//同上

		for (csGameData.GetCurrentFoodNum(); csGameData.GetCurrentFoodNum() < lProduceNum; csGameData.IncCurrentFoodNum())
		{
			do
			{
				stFood.x = csDistX(csRandom);
				stFood.y = csDistY(csRandom);
			} while (csGameData.GetMap(stFood) != NULL_BLOCK);//遇到空地为止

			csGameData.GetMap(stFood) = FOOD_BLOCK;
		}
	}

	static bool Move(Game_Data &csGameData)
	{
		//先将蛇头向当前方向移动一格
		My_Point stNewSnakeHead = csGameData.GetSnakeHead();
		stNewSnakeHead += Game_Data::stSnakeMove[(long)csGameData.GetMoveDirect()];

		//判断是否输了
		if (Cross(csGameData, stNewSnakeHead) || csGameData.GetMap(stNewSnakeHead) > NULL_BLOCK)//撞墙或吃到自身
		{
			return false;
		}

		//递增移动距离
		csGameData.IncTravelDistance();

		//判断是否吃到食物
		if (csGameData.GetMap(stNewSnakeHead) == FOOD_BLOCK)
		{
			//递减食物数
			csGameData.DecCurrentFoodNum();
			//更新新蛇头为原先蛇头的值+1，并不移动蛇尾
			csGameData.GetMap(stNewSnakeHead) = csGameData.GetSnakeLength() + 1;
			//设置新蛇头位置
			csGameData.GetSnakeHead() = stNewSnakeHead;
			//生成新的食物
			if (!csGameData.GetEatAllToProduce() || csGameData.GetCurrentFoodNum() == 0)
			{//如果不是吃完全部再生成那么直接生成，否则判断当前食物数是否为0（短路求值特性，左边为false才测试右侧值）
				ProduceFood(csGameData);
			}
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
				stMove += Game_Data::stSnakeMove[j];//移动

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