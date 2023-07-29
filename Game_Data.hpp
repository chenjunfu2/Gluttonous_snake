#pragma once

#include "My_Point.hpp"
#include "Game_Sign.hpp"

//#include <stdlib.h>
#include <utility>

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
	long *lMap;//��ͼ

	long lMoveInterval;//�ƶ����
	My_Point stSnakeHead;//ͷ����
	Move_Direct enMoveDirect;//�ƶ�����

public:
	Game_Data(long _lMapWidth, long _lMapHigh, long _lMoveInterval) :
		lMapWidth(_lMapWidth), lMapHigh(_lMapHigh), lMoveInterval(_lMoveInterval), stSnakeHead({0,0}), enMoveDirect(Move_Direct::Right)
	{
		lMap = new long[lMapWidth * lMapHigh];
	}

	~Game_Data(void)
	{
		delete[] lMap;
		lMap = NULL;
	}

	Game_Data(Game_Data &&_Move) :
		lMapWidth(_Move.lMapWidth),
		lMapHigh(_Move.lMapHigh),
		lMap(_Move.lMap),
		lMoveInterval(_Move.lMoveInterval),
		stSnakeHead(std::move(_Move.stSnakeHead)),
		enMoveDirect(_Move.enMoveDirect)
	{
		_Move.lMapWidth = 0;
		_Move.lMapHigh = 0;
		_Move.lMap = NULL;
		_Move.lMoveInterval = 0;
		_Move.enMoveDirect = Move_Direct::No_Move;
	}

	void Reset(const My_Point &_stSnakeBegin = {0,0}, Move_Direct _enDirectBegin = Move_Direct::Right)
	{
		stSnakeHead = _stSnakeBegin;
		enMoveDirect = _enDirectBegin;

		memset(lMap, 0, lMapWidth * lMapHigh * sizeof(long));
		GetMap(stSnakeHead) = SNAKE_TAIL;//��ʼ����ͼ����ͷλ�õ�ֵ
	}

	long &GetMoveInterval(void)
	{
		return lMoveInterval;
	}

	const long &GetMoveInterval(void) const
	{
		return lMoveInterval;
	}

	long GetSnakeLength(void) const
	{
		return GetMap(stSnakeHead);//��ͷ��ָ��Ϊ�߳���
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