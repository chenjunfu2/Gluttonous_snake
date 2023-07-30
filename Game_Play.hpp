#pragma once

#include "Game_Control.hpp"
#include "Game_Draw.hpp"

#include <random>

class Game_Play
{
private:
	Game_Data csGameData;
	std::mt19937 csRandom;
	My_Point stSnakeHeadBegin;
	Game_Data::Move_Direct enMoveDirectBegin;
private:
	void Reset(void)
	{
		csGameData.ResetMap();//���õ�ͼ����
		csGameData.ResetTravelDistance();//�����ƶ�����
		csGameData.ResetCurrentFoodNum();//���õ�ǰʳ����
		csGameData.GetSnakeHead() = stSnakeHeadBegin;//��ʼ����λ��
		csGameData.GetMoveDirect() = enMoveDirectBegin;//��ʼ���߷���
		csGameData.GetMap(csGameData.GetSnakeHead()) = SNAKE_TAIL;//��ʼ����ͼ����ͷλ�õ�ֵ
	}
public:
	Game_Play(Game_Data &&_csSnakeData, unsigned int uiRandSeed, const My_Point &_stSnakeHeadBegin = {0,0}, Game_Data::Move_Direct _enMoveDirectBegin = Game_Data::Move_Direct::Right) :
		csGameData(std::move(_csSnakeData)), csRandom(uiRandSeed), stSnakeHeadBegin(_stSnakeHeadBegin), enMoveDirectBegin(_enMoveDirectBegin)
	{
		//�������ģʽ
		Game_Control::SetOutputFullMode(csGameData);
	}

	~Game_Play(void)
	{
		//�������ģʽ
		Game_Control::SetOutputLineMode();
	}

	const Game_Data &GetSnakeData(void) const
	{
		return csGameData;
	}

	void Init(void)
	{
		Game_Draw::Clear();//����
		Game_Draw::Start();//�����ʼ��Ϣ
		Game_Control::GetAnyKey();//�����������
		Game_Draw::Clear();//����
		
		//��������
		Reset();
		//��ʼ��ʳ��
		Game_Control::ProduceFood(csGameData, csRandom);
	}

	std::mt19937 &GetRandom(void)
	{
		return csRandom;
	}

	void SetRandomSeed(unsigned int uiRandSeed = 0)
	{
		csRandom = std::mt19937(uiRandSeed);
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
		while (true)
		{
			Game_Control::Wait(csGameData.GetMoveInterval());
			Game_Control::ChangeDirect(csGameData, Game_Control::Input());
			bool bLose = !Game_Control::Move(csGameData, csRandom);
			Game_Draw::Interface(csGameData);
			Game_Draw::Info(csGameData);

			if (bLose)
			{
				Game_Draw::Lose();
				break;
			}
			else if (Game_Control::Win(csGameData))
			{
				Game_Draw::Win();
				break;
			}
		}

		Game_Draw::End("N", "L");
		return Game_Control::SwitchInput("Nn", 2, "Ll", 2);
	}
};