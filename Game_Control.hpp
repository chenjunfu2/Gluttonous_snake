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
		//Խ��
		if (stNewSnakeHead.x >= csGameData.GetMapWidth() ||
			stNewSnakeHead.y >= csGameData.GetMapHigh() ||
			stNewSnakeHead.x < 0 ||
			stNewSnakeHead.y < 0)
		{
			return true;
		}
		return false;
	}

	static constexpr My_Point stSnakeMove[(long)Game_Data::Move_Direct::Arr_End] =//�ƶ�����
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
		while (_kbhit() != 0)//����һ������
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
			case 'p'://��ͣ
			case 'P':
				{
					int iGet;
					do
					{
						iGet = _getch();
					} while (iGet != 'p' && iGet != 'P');
					Wait(50);//��50ms��Ӧʱ��
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
		//���û�����Ϊȫ���壬��СΪ��ͼ��С
		setvbuf(stdout, NULL, _IOFBF, (csGameData.GetMapHigh() + 2) * (csGameData.GetMapWidth() + 2) + 1);
	}

	static void SetOutputLineMode(void)
	{
		setvbuf(stdout, NULL, _IOLBF, BUFSIZ);//�ָ�Ĭ��
	}

	static void ChangeDirect(Game_Data &csGameData, Game_Data::Move_Direct enMoveDirect)
	{
		//�ж���û�н����ƶ�
		if (enMoveDirect != Game_Data::Move_Direct::No_Move)
		{
			//�жϸı�ķ����Ƿ�������
			My_Point stCurrent = csGameData.GetSnakeHead();
			stCurrent += stSnakeMove[(long)enMoveDirect];
			if (csGameData.GetMap(stCurrent) == 0 || csGameData.GetMap(stCurrent) + 1 != csGameData.GetSnakeLength())//��������
			{
				csGameData.GetMoveDirect() = enMoveDirect;//�ı䷽��
			}
		}
	}

	static void ProduceFood(Game_Data &csGameData, std::mt19937 csRandom)//�����޸�������
	{
		if (csGameData.GetSnakeLength() >= csGameData.GetMapHigh() * csGameData.GetMapWidth())
		{
			return;//����ʧ��
		}

		My_Point stFood;
		std::uniform_int_distribution<long> csDistX(0, csGameData.GetMapWidth() - 1);//max��ȡ��
		std::uniform_int_distribution<long> csDistY(0, csGameData.GetMapHigh() - 1);//����Ҫ-1
		do
		{
			stFood.x = csDistX(csRandom);
			stFood.y = csDistY(csRandom);
		} while (csGameData.GetMap(stFood) != 0);//�����յ�Ϊֹ

		csGameData.GetMap(stFood) = FOOD_BLOCK;
	}

	static bool Move(Game_Data &csGameData, std::mt19937 &csRandom)
	{
		//�Ƚ���ͷ��ǰ�����ƶ�һ��
		My_Point stNewSnakeHead = csGameData.GetSnakeHead();
		stNewSnakeHead += stSnakeMove[(long)csGameData.GetMoveDirect()];

		//�ж��Ƿ�����
		if (Cross(csGameData, stNewSnakeHead) || csGameData.GetMap(stNewSnakeHead) > NULL_BLOCK)//ײǽ��Ե�����
		{
			return false;
		}

		//�ж��Ƿ�Ե�ʳ��
		if (csGameData.GetMap(stNewSnakeHead) == FOOD_BLOCK)
		{
			//��������ͷΪԭ����ͷ��ֵ+1�������ƶ���β
			csGameData.GetMap(stNewSnakeHead) = csGameData.GetSnakeLength() + 1;
			//��������ͷλ��
			csGameData.GetSnakeHead() = stNewSnakeHead;
			//�����µ�ʳ��
			ProduceFood(csGameData, csRandom);

			return true;
		}

		//��������ͷΪԭ����ͷ��ֵ
		csGameData.GetMap(stNewSnakeHead) = csGameData.GetSnakeLength();
		//��������ͷλ��
		csGameData.GetSnakeHead() = stNewSnakeHead;

		//������������β
		My_Point stCurrent = stNewSnakeHead;//������ֵΪ��β����
		while (csGameData.GetMap(stCurrent) != NULL_BLOCK)
		{
			//�鿴�ĸ�������������ֵ��ͬ��ֵ
			for (long j = (long)Game_Data::Move_Direct::Arr_Beg; j < (long)Game_Data::Move_Direct::Arr_End; ++j)
			{
				My_Point stMove = stCurrent;
				stMove += stSnakeMove[j];//�ƶ�

				if (!Cross(csGameData, stMove) && csGameData.GetMap(stMove) == csGameData.GetMap(stCurrent))//��ͬ
				{
					--csGameData.GetMap(stMove);//�ݼ�
					stCurrent = stMove;//�ƶ�
					break;//������һ��λ��
				}
			}
		}

		return true;
	}
};