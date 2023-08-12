#pragma once

#include "My_Point.hpp"

#include <stdlib.h>
#include <utility>
#include <stdexcept>
#include <random>
#include <algorithm>

class Game_Data
{
public:
	enum class Move_Direct :unsigned short
	{
		Beg = 0,
		No_Move = Beg,
		Up,
		Down,
		Left,
		Right,
		End,
	};

	static Move_Direct GetNegativeDirection(Move_Direct enMoveDirect)
	{
		static constexpr const Move_Direct NegativeDirection[(long)Move_Direct::End] =
		{
			Move_Direct::No_Move,
			Move_Direct::Down,
			Move_Direct::Up,
			Move_Direct::Right,
			Move_Direct::Left,
		};

		return NegativeDirection[(long)enMoveDirect];
	}

	enum class Map_Type :unsigned short//修改此处顺序记得修改所有使用其的函数
	{
		Beg = 0,
		Blank = Beg,//空白
		Head,//头
		Body,//身
		Tail,//尾
		Wall,//墙壁
		Food,//食物
		End,
	};

	static constexpr My_Point stSnakeMove[(long)Game_Data::Move_Direct::End] =//移动方向
	{
		{ 0, 0},
		{ 0,-1},
		{ 0, 1},
		{-1, 0},
		{ 1, 0},
	};

	struct Map
	{
		Move_Direct enMoveDirect = Move_Direct::No_Move;
		Map_Type enMapType = Map_Type::Blank;
	};
private:
	long lMapWidth;//x
	long lMapHigh;//y
	Map *stArrMap;//地图,高4字节存储蛇部位信息，低4字节存储部位指向信息

	long lMoveInterval;//移动间隔
	long lWinLength;//获胜长度
	long lSnakeLength;//蛇当前长度
	long lFoodMaxNum;//地图中最多出现的食物个数
	long lFoodNum;//地图中当前的食物个数
	long lTravelDistance;//移动距离

	My_Point stSnakeHead;//头坐标
	My_Point stSnakeTail;//尾坐标
	//Move_Direct enHeadDirect;//头方向
	std::mt19937 csRandom;//随机数生成器

	bool bEatAllToProduce;//食物生成策略：吃掉所有食物生成/吃掉一个生成一个
public:
	Game_Data(long _lMapWidth, long _lMapHigh, long _lMoveInterval, long _lWinLength = -1, long _lFoodMaxNum = 1, unsigned int _uiRandomSeed = 0, bool _bEatAllToProduce = true) :
		lMapWidth		(_lMapWidth), 
		lMapHigh		(_lMapHigh), 
		lMoveInterval	(_lMoveInterval),
		lWinLength		(_lWinLength),
		lSnakeLength	(0),
		lFoodMaxNum		(_lFoodMaxNum),
		lFoodNum		(0),
		lTravelDistance	(0),
		stSnakeHead		({1,0}),
		stSnakeTail		({0,0}),
		//enHeadDirect	(Move_Direct::Right),
		csRandom		(_uiRandomSeed),
		bEatAllToProduce(_bEatAllToProduce)
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

		stArrMap = new Map[lMapWidth * lMapHigh];
	}

	~Game_Data(void)
	{
		delete[] stArrMap;
		stArrMap = NULL;
	}

	Game_Data(Game_Data &&_Move) noexcept :
		lMapWidth		(_Move.lMapWidth),
		lMapHigh		(_Move.lMapHigh),
		stArrMap		(_Move.stArrMap),
		lMoveInterval	(_Move.lMoveInterval),
		lWinLength		(_Move.lWinLength),
		lSnakeLength	(_Move.lSnakeLength),
		lFoodMaxNum		(_Move.lFoodMaxNum),
		lFoodNum		(_Move.lFoodNum),
		lTravelDistance	(_Move.lTravelDistance),
		stSnakeHead		(std::move(_Move.stSnakeHead)),
		stSnakeTail		(std::move(_Move.stSnakeTail)),
		//enHeadDirect	(_Move.enHeadDirect),
		csRandom		(std::move(_Move.csRandom)),
		bEatAllToProduce(_Move.bEatAllToProduce)
	{
		_Move.lMapWidth = 0;
		_Move.lMapHigh = 0;
		_Move.stArrMap = NULL;
		_Move.lMoveInterval = 0;
		_Move.lWinLength = 0;
		_Move.lSnakeLength = 0;
		_Move.lFoodMaxNum = 0;
		_Move.lFoodNum = 0;
		_Move.lTravelDistance = 0;
		//_Move.stSnakeHead 通过std::move，无需重置
		//_Move.stSnakeTail 通过std::move，无需重置
		//_Move.enHeadDirect = Move_Direct::No_Move;
		//_Move.csRandom 通过std::move，无需重置
		_Move.bEatAllToProduce = false;
	}

	void ResizeMap(long _lMapWidth, long _lMapHigh)
	{
		if (_lMapWidth <= 0 || _lMapHigh <= 0)
		{
			throw std::invalid_argument("错误的地图大小");
		}

		Map *ulArrNewMap = new Map[_lMapWidth * _lMapHigh];
		delete[] stArrMap;
		stArrMap = ulArrNewMap;
		lMapWidth = _lMapWidth, lMapHigh = _lMapHigh;
	}

	void ResetMap(void)
	{
		memset(stArrMap, 0, lMapWidth * lMapHigh * sizeof(Map));
	}

	bool CrossMap(const My_Point &stPoint)//越界判断
	{
		//越界
		if (stPoint.x >= lMapWidth ||
			stPoint.y >= lMapHigh ||
			stPoint.x < 0 ||
			stPoint.y < 0)
		{
			return true;
		}
		return false;
	}

	void Surround(My_Point &stPoint)//越界环绕
	{
		if (CrossMap(stPoint))
		{
			stPoint.x += lMapWidth;//发生负数环绕时转变为正数
			stPoint.x %= lMapWidth;//发生正数环绕时求模，上面加法只改变除法结果，不改变余数

			stPoint.y += lMapHigh;//对y的处理同上对x的处理
			stPoint.y %= lMapHigh;
		}
	}

	void ResetSnake(My_Point stSnakeHeadBegin, Move_Direct enMoveDirectBegin) 
	{
		//设置成员
		stSnakeHead = stSnakeHeadBegin;
		//enHeadDirect = enMoveDirectBegin;

		//判断是否越界，越界则放入最靠近越界点的地图边界内
		if (CrossMap(stSnakeHead))
		{
			stSnakeHead.x = std::max<long>(0, stSnakeHead.x);
			stSnakeHead.y = std::max<long>(0, stSnakeHead.y);

			stSnakeHead.x = std::min<long>(stSnakeHead.x, lMapWidth - 1);
			stSnakeHead.y = std::min<long>(stSnakeHead.y, lMapHigh - 1);
		}
		
		//计算尾坐标在头部方向的反方向
		stSnakeTail = stSnakeHead + stSnakeMove[(long)GetNegativeDirection(enMoveDirectBegin)];

		//判断是否越界
		if (CrossMap(stSnakeTail))
		{
			//处理尾部越界问题
			stSnakeHead += stSnakeMove[(long)enMoveDirectBegin];
			stSnakeTail += stSnakeMove[(long)enMoveDirectBegin];
			/*
			原理：首先蛇头保证不越界，其次蛇尾在蛇头朝向的反方向一格处，
			若蛇尾越界，则代表蛇尾刚好在边界上，那么整条蛇向着当前方向移动一格
			就能使蛇尾移出边界（注：地图大小至少为2*2，移动不会使蛇头越界）
			上		下
			∩		∧
			∨		∪

			左		右
			⊂＞		＜⊃
			*/
		}

		//设置蛇头指向移动方向
		GetMap(stSnakeHead) = Map{enMoveDirectBegin,Map_Type::Head};
		//设置蛇尾指向蛇头
		GetMap(stSnakeTail) = Map{enMoveDirectBegin,Map_Type::Tail};
		//设置蛇长度
		lSnakeLength = 2;//头+尾=2
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

	void IncFoodNum(void)
	{
		++lFoodNum;
	}

	void DecFoodNum(void)
	{
		--lFoodNum;
	}

	void ResetFoodNum(void)
	{
		lFoodNum = 0;
	}

	long &GetFoodNum(void)
	{
		return lFoodNum;
	}

	const long &GetFoodNum(void) const
	{
		return lFoodNum;
	}

	void IncTravelDistance(void)
	{
		++lTravelDistance;
	}

	void ResetTravelDistance(void)
	{
		lTravelDistance = 0;
	}

	long &GetTravelDistance(void)
	{
		return lTravelDistance;
	}

	const long &GetTravelDistance(void) const
	{
		return lTravelDistance;
	}

	void IncSnakeLength(void)
	{
		++lSnakeLength;
	}

	void ResetSnakeLenght(void)
	{
		lSnakeLength = 0;
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

	My_Point &GetSnakeTail(void)
	{
		return stSnakeTail;
	}

	const My_Point &GetSnakeTail(void) const
	{
		return stSnakeTail;
	}

	Move_Direct &GetHeadDirect(void)//蛇头方向可以主动修改
	{
		return GetMap(GetSnakeHead()).enMoveDirect;
	}

	const Move_Direct &GetHeadDirect(void) const
	{
		return GetMap(GetSnakeHead()).enMoveDirect;
	}

	const Move_Direct &GetTailDirect(void) const//蛇尾方向不可主动修改
	{
		return GetMap(GetSnakeTail()).enMoveDirect;
	}

	std::mt19937 &GetRandom(void)
	{
		return csRandom;
	}

	const std::mt19937 &GetRandom(void) const
	{
		return csRandom;
	}

	void SetRandomSeed(unsigned int uiRandomSeed)
	{
		csRandom = std::mt19937(uiRandomSeed);
	}

	bool &GetEatAllToProduce(void)
	{
		return bEatAllToProduce;
	}

	const bool &GetEatAllToProduce(void) const
	{
		return bEatAllToProduce;
	}

	Map &GetMap(long x, long y)
	{
		return stArrMap[y * lMapWidth + x];
	}

	const Map &GetMap(long x, long y)const
	{
		return stArrMap[y * lMapWidth + x];
	}

	Map &GetMap(const My_Point &stPoint)
	{
		return GetMap(stPoint.x, stPoint.y);
	}

	const Map &GetMap(const My_Point &stPoint) const
	{
		return GetMap(stPoint.x, stPoint.y);
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