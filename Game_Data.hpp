#pragma once

#include "My_Point.hpp"
#include "Game_Sign.hpp"

#include <stdlib.h>
#include <utility>
#include <stdexcept>

class Game_Data
{
public:
	enum class Move_Direct :long
	{
		Arr_Beg = 0, 
		Up = Arr_Beg,
		Down,
		Left,
		Right,
		Arr_End,
		No_Move = Arr_End,
	};

private:
	long lMapWidth;//x
	long lMapHigh;//y
	long *lMap;//地图

	long lMoveInterval;//移动间隔
	long lWinLength;//获胜长度
	long lFoodMaxNum;//地图中最多出现的食物个数
	long lCurrentFoodNum;//地图中当前的食物个数
	long lTravelDistance;//移动距离
	My_Point stSnakeHead;//头坐标
	Move_Direct enMoveDirect;//移动方向
public:
	Game_Data(long _lMapWidth, long _lMapHigh, long _lMoveInterval, long _lWinLength = -1, long _lFoodMaxNum = 1) :
		lMapWidth		(_lMapWidth), 
		lMapHigh		(_lMapHigh), 
		lMoveInterval	(_lMoveInterval),
		lWinLength		(_lWinLength),
		lFoodMaxNum		(_lFoodMaxNum),
		lCurrentFoodNum	(0),
		lTravelDistance	(0),
		stSnakeHead		({0,0}), 
		enMoveDirect	(Move_Direct::Right)
	{
		if (lMapWidth <= 0 || lMapHigh <= 0)
		{
			throw std::invalid_argument("错误的地图大小");
		}
		if (lMoveInterval < 0)
		{
			throw std::invalid_argument("错误的时间间隔");
		}
		if (lWinLength < 0)
		{
			if (lWinLength == -1)
			{
				lWinLength = lMapWidth * lMapHigh;
			}
			else
			{
				throw std::invalid_argument("错误的获胜长度");
			}
		}
		if (lFoodMaxNum < 0)
		{
			throw std::invalid_argument("错误的最大食物数");
		}

		lMap = new long[lMapWidth * lMapHigh];
	}

	~Game_Data(void)
	{
		delete[] lMap;
		lMap = NULL;
	}

	Game_Data(Game_Data &&_Move) noexcept :
		lMapWidth		(_Move.lMapWidth),
		lMapHigh		(_Move.lMapHigh),
		lMap			(_Move.lMap),
		lMoveInterval	(_Move.lMoveInterval),
		lWinLength		(_Move.lWinLength),
		lFoodMaxNum		(_Move.lFoodMaxNum),
		lCurrentFoodNum	(_Move.lCurrentFoodNum),
		lTravelDistance	(_Move.lTravelDistance),
		stSnakeHead		(std::move(_Move.stSnakeHead)),
		enMoveDirect	(_Move.enMoveDirect)
	{
		_Move.lMapWidth = 0;
		_Move.lMapHigh = 0;
		_Move.lMap = NULL;
		_Move.lMoveInterval = 0;
		_Move.lWinLength = 0;
		_Move.lFoodMaxNum = 0;
		_Move.lCurrentFoodNum = 0;
		_Move.lTravelDistance = 0;
		//_Move.stSnakeHead 通过std::move，无需重置
		_Move.enMoveDirect = Move_Direct::No_Move;
	}

	void ResizeMap(long _lMapWidth, long _lMapHigh)
	{
		if (_lMapWidth <= 0 || _lMapHigh <= 0)
		{
			throw std::invalid_argument("错误的地图大小");
		}

		long *lNewMap = new long[_lMapWidth * _lMapHigh];
		delete[] lMap;
		lMap = lNewMap;
		lMapWidth = _lMapWidth, lMapHigh = _lMapHigh;
	}

	void ResetMap(void)
	{
		memset(lMap, 0, lMapWidth * lMapHigh * sizeof(long));
	}

	long &GetMoveInterval(void)
	{
		return lMoveInterval;
	}

	const long &GetMoveInterval(void) const
	{
		return lMoveInterval;
	}

	long &GetWinLength(void)
	{
		return lWinLength;
	}

	const long &GetWinLength(void) const
	{
		return lWinLength;
	}

	long &GetFoodMaxNum(void)
	{
		return lFoodMaxNum;
	}

	const long &GetFoodMaxNum(void) const
	{
		return lFoodMaxNum;
	}

	long &IncCurrentFoodNum(void)
	{
		return ++lCurrentFoodNum;
	}

	long &DecCurrentFoodNum(void)
	{
		return --lCurrentFoodNum;
	}

	long &ResetCurrentFoodNum(void)
	{
		return lCurrentFoodNum = 0;
	}

	long &GetCurrentFoodNum(void)
	{
		return lCurrentFoodNum;
	}

	const long &GetCurrentFoodNum(void) const
	{
		return lCurrentFoodNum;
	}

	long &IncTravelDistance(void)
	{
		return ++lTravelDistance;
	}

	long &ResetTravelDistance(void)
	{
		return lTravelDistance = 0;
	}

	long &GetTravelDistance(void)
	{
		return lTravelDistance;
	}

	const long &GetTravelDistance(void) const
	{
		return lTravelDistance;
	}

	long GetSnakeLength(void) const
	{
		return GetMap(stSnakeHead);//蛇头数指即为蛇长度
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