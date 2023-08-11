#pragma once

#include "Game_Control.hpp"
#include "Game_Draw.hpp"

#include <random>

class Game_Play
{
private:
	Game_Data csGameData;
	My_Point stSnakeHeadBegin;
	Game_Data::Move_Direct enMoveDirectBegin;
private:
	void Reset(void)
	{
		csGameData.ResetMap();//重置地图数据
		csGameData.ResetSnakeLenght();//重置蛇长度
		csGameData.ResetFoodNum();//重置当前食物数
		csGameData.ResetTravelDistance();//重置移动距离

		csGameData.ResetSnake(stSnakeHeadBegin, enMoveDirectBegin);//初始化蛇
	}
public:
	Game_Play(Game_Data &&_csSnakeData, const My_Point &_stSnakeHeadBegin = {0,0}, Game_Data::Move_Direct _enMoveDirectBegin = Game_Data::Move_Direct::Right) :
		csGameData(std::move(_csSnakeData)), stSnakeHeadBegin(_stSnakeHeadBegin), enMoveDirectBegin(_enMoveDirectBegin)
	{
		//设置IO模式
		Game_Control::SetIOMode(csGameData);
	}

	~Game_Play(void) = default;

	const Game_Data &GetSnakeData(void) const
	{
		return csGameData;
	}

	void Init(void)
	{
		Game_Draw::Clear();//清屏
		Game_Draw::Start();//输出开始信息
		Game_Control::GetAnyKey();//按任意键继续
		Game_Draw::Clear();//清屏
		
		//重置数据
		Reset();
		//初始化食物
		Game_Control::ProduceFood(csGameData);
	}

	std::mt19937 &GetRandom(void)
	{
		return csGameData.GetRandom();
	}

	void SetRandomSeed(unsigned int uiRandomSeed = 0)
	{
		csGameData.SetRandomSeed(uiRandomSeed);
	}

	My_Point &GetSnakeHeadBegin(void)
	{
		return stSnakeHeadBegin;
	}

	Game_Data::Move_Direct &GetMoveDirectBegin(void)
	{
		return enMoveDirectBegin;
	}

	bool Loop(void)
	{
		//显示界面
		Game_Draw::Interface(csGameData);
		Game_Draw::Info(csGameData);

		//游戏循环
		while (true)
		{
			Game_Control::Wait(csGameData.GetMoveInterval());
			Game_Control::ChangeDirect(csGameData, Game_Control::Input());
			bool bLose = !Game_Control::Move(csGameData);
			Game_Draw::Interface(csGameData);
			Game_Draw::Info(csGameData);

			if (bLose)
			{
				Game_Draw::Lose();
				break;
			}

			if (Game_Control::Win(csGameData))
			{
				Game_Draw::Win();
				break;
			}
		}

		Game_Draw::End(L"N", L"L");
		return Game_Control::SwitchInput(L"Nn", 2, L"Ll", 2);
	}
};