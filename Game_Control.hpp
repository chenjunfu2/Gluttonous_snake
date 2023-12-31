﻿#pragma once

#include "Game_Data.hpp"

#include <conio.h>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>

class Game_Control
{
	static bool Lose(const Game_Data &csGameData, My_Point stNewSnakeHead)
	{
		Game_Data::Map_Type enMapType = csGameData.GetMap(stNewSnakeHead).enMapType;

		if (enMapType == Game_Data::Map_Type::Head ||
			enMapType == Game_Data::Map_Type::Body ||
			//enMapType == Game_Data::Map_Type::Tail ||//允许碰到蛇尾，因为蛇在运动时蛇头紧跟着蛇尾时实际上是同时移动的，并不会吃掉
			enMapType == Game_Data::Map_Type::Wall)//撞墙或吃掉自身，游戏结束
		{
			return true;
		}

		return false;
	}
public:
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


	enum class CgDir_Status :long
	{
		No_Change,
		Change,
		Add_Speed,
	};

	static CgDir_Status ChangeDirect(Game_Data &csGameData, Game_Data::Move_Direct enMoveDirect)//返回0：没有移动 返回1：成功移动 返回2：加速移动
	{
		//判断有没有进行移动
		if (enMoveDirect == Game_Data::Move_Direct::No_Move)
		{
			return CgDir_Status::No_Change;
		}

		//禁止反向扭头
		if (enMoveDirect == Game_Data::GetNegativeDirection(csGameData.GetHeadDirect()))
		{
			return CgDir_Status::No_Change;
		}

		//判断是否加速移动
		if (enMoveDirect == csGameData.GetHeadDirect())
		{
			return CgDir_Status::Add_Speed;//加速移动
		}
		
		csGameData.GetHeadDirect() = enMoveDirect;//改变方向
		return CgDir_Status::Change;
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

		for (csGameData.GetFoodNum(); csGameData.GetFoodNum() < lProduceNum; csGameData.IncFoodNum())
		{
			do
			{
				stFood.x = csDistX(csRandom);
				stFood.y = csDistY(csRandom);
			} while (csGameData.GetMap(stFood).enMapType != Game_Data::Map_Type::Blank);//遇到空地为止

			csGameData.GetMap(stFood) = Game_Data::Map{Game_Data::Move_Direct::No_Move,Game_Data::Map_Type::Food};
		}
	}

	static bool Move(Game_Data &csGameData)//移动出边界后环绕
	{
		//保存当前蛇头方向
		Game_Data::Move_Direct enCurrentHeadDirect = csGameData.GetHeadDirect();
		//获取蛇头向当前方向移动一格的位置
		My_Point stNewSnakeHead = csGameData.GetSnakeHead() + Game_Data::stSnakeMove[(long)enCurrentHeadDirect];
		//越界环绕，环绕只需处理蛇头与蛇尾，蛇身部分无需改变
		csGameData.Surround(stNewSnakeHead);

		//判断是否输了
		if (Lose(csGameData, stNewSnakeHead))
		{
			return false;//直接返回，剩下都不需要更新
		}

		//递增移动距离
		csGameData.IncTravelDistance();
		//修改地图前判断是否吃到食物
		bool bEatFood = csGameData.GetMap(stNewSnakeHead).enMapType == Game_Data::Map_Type::Food;

		//没有吃到食物才清除蛇尾
		My_Point stNewSnakeTail;
		if (!bEatFood)
		{
			//保存当前蛇尾方向
			Game_Data::Move_Direct enCurrentTailDirect = csGameData.GetTailDirect();

			//获取蛇尾向当前方向移动一格的位置
			stNewSnakeTail = csGameData.GetSnakeTail() + Game_Data::stSnakeMove[(long)enCurrentTailDirect];
			//越界环绕
			csGameData.Surround(stNewSnakeTail);

			//清除原先蛇尾位置下的数据、类型
			csGameData.GetMap(csGameData.GetSnakeTail()) = Game_Data::Map
			{
				Game_Data::Move_Direct::No_Move,
				Game_Data::Map_Type::Blank,
			};
		}

		//清除原先蛇头位置并更新蛇头，这个操作在清除蛇尾之后，避免头尾相交改变蛇尾数据

		//设置原先蛇头位置下的数据、类型
		csGameData.GetMap(csGameData.GetSnakeHead()) = Game_Data::Map
		{
			enCurrentHeadDirect,
			Game_Data::Map_Type::Body,//（如果只有两格的情况下，尾部会将此身体修改为尾部，如果吃到食物则此身体不用改变）
		};

		//设置新蛇头位置下的数据、类型
		csGameData.GetMap(stNewSnakeHead) = Game_Data::Map
		{
			enCurrentHeadDirect,
			Game_Data::Map_Type::Head,
		};

		//设置新蛇头位置
		csGameData.GetSnakeHead() = stNewSnakeHead;

		//没有吃到食物最后才设置新蛇尾
		if (!bEatFood)
		{
			//设置新蛇尾位置下的类型，数据不变
			csGameData.GetMap(stNewSnakeTail).enMapType = Game_Data::Map_Type::Tail;

			//设置新蛇尾位置
			csGameData.GetSnakeTail() = stNewSnakeTail;
		}
		else//吃到食物的处理
		{
			//递增蛇长度
			csGameData.IncSnakeLength();
			//递减食物数
			csGameData.DecFoodNum();

			//生成新的食物
			if (!csGameData.GetEatAllToProduce() || csGameData.GetFoodNum() == 0)
			{//如果不是吃完全部再生成那么直接生成，否则判断当前食物数是否为0（短路求值特性，左边为false才测试右侧值）
				ProduceFood(csGameData);
			}
		}
		
		return true;
	}
};