#pragma once

#include "Game_Control.hpp"
#include "Game_Draw.hpp"

#include <random>

class Play_Game
{
private:
	Game_Data csGameData;
	std::mt19937 csRandom;
public:
	Play_Game(Game_Data &&_csSnakeData) :
		csGameData(std::move(_csSnakeData))
	{
		//设置输出模式
		Control::SetOutputFullMode(csGameData);
	}

	~Play_Game(void)
	{
		//设置输出模式
		Control::SetOutputLineMode();
	}

	Game_Data &GetSnakeData(void)
	{
		return csGameData;
	}

	void Init(const My_Point &_stSnakeBegin = {0,0}, Game_Data::Move_Direct _enDirectBegin = Game_Data::Move_Direct::Right, unsigned int uiRandSeed = 0)
	{
		Game_Draw::Clear();//清屏
		Game_Draw::Info();//输出开始信息
		Control::GetAnyKey();//按任意键继续
		Game_Draw::Clear();//清屏

		//初始化随机数发生器
		csRandom = std::mt19937(uiRandSeed);
		//重置数据
		csGameData.Reset(_stSnakeBegin, _enDirectBegin);
		//初始化食物
		Control::ProduceFood(csGameData, csRandom);
	}

	bool Loop(void)
	{
		while (true)
		{
			Control::Wait(csGameData.GetMoveInterval());
			Control::ChangeDirect(csGameData, Control::Input());

			if (!Control::Move(csGameData, csRandom))
			{
				Game_Draw::Draw(csGameData);
				Game_Draw::Lose();
				break;
			}

			Game_Draw::Draw(csGameData);

			if (Control::Win(csGameData))
			{
				Game_Draw::Win();
				break;
			}
		}

		Game_Draw::End("N", "L");
		return Control::SwitchInput("Nn", 2, "Ll", 2);
	}
};