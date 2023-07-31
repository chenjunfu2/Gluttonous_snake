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
		while (_kbhit() != 0)//����һ������
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
			case 'p'://��ͣ
			case 'P':
				{
					int iGet;
					do
					{
						iGet = _getwch();
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
		//���û�����Ϊȫ���壬��СΪ��ͼ��С
		setvbuf(stdout, NULL, _IOFBF, (csGameData.GetMapHigh() + 2) * (csGameData.GetMapWidth() + 2) + 1);
		(void)_setmode(_fileno(stdout), _O_U16TEXT);//��������ַ���
		(void)_setmode(_fileno(stdin), _O_U16TEXT);//���������ַ���
	}

	static void ChangeDirect(Game_Data &csGameData, Game_Data::Move_Direct enMoveDirect)
	{
		//�ж���û�н����ƶ�
		if (enMoveDirect != Game_Data::Move_Direct::No_Move)
		{
			//�жϸı�ķ����Ƿ�������
			My_Point stCurrent = csGameData.GetSnakeHead();
			stCurrent += Game_Data::stSnakeMove[(long)enMoveDirect];
			if (csGameData.GetMap(stCurrent) == 0 || csGameData.GetMap(stCurrent) + 1 != csGameData.GetSnakeLength())//��������
			{
				csGameData.GetMoveDirect() = enMoveDirect;//�ı䷽��
			}
		}
	}

	static void ProduceFood(Game_Data &csGameData)//�����޸�������
	{
		long lFreeSpace = csGameData.GetMapHigh() * csGameData.GetMapWidth() - csGameData.GetSnakeLength();
		long lProduceNum = min(lFreeSpace, csGameData.GetFoodMaxNum());//��ȡ��λ��Сֵ�����ɴ�С��Сֵ

		if (lProduceNum <= 0)//��ͼû�ռ���
		{
			return;//����ʧ��
		}

		My_Point stFood;
		std::mt19937 &csRandom = csGameData.GetRandom();
		std::uniform_int_distribution<long> csDistX(0, csGameData.GetMapWidth() - 1);//max��ȡ������Ҫ-1
		std::uniform_int_distribution<long> csDistY(0, csGameData.GetMapHigh() - 1);//ͬ��

		for (csGameData.GetCurrentFoodNum(); csGameData.GetCurrentFoodNum() < lProduceNum; csGameData.IncCurrentFoodNum())
		{
			do
			{
				stFood.x = csDistX(csRandom);
				stFood.y = csDistY(csRandom);
			} while (csGameData.GetMap(stFood) != NULL_BLOCK);//�����յ�Ϊֹ

			csGameData.GetMap(stFood) = FOOD_BLOCK;
		}
	}

	static bool Move(Game_Data &csGameData)
	{
		//�Ƚ���ͷ��ǰ�����ƶ�һ��
		My_Point stNewSnakeHead = csGameData.GetSnakeHead();
		stNewSnakeHead += Game_Data::stSnakeMove[(long)csGameData.GetMoveDirect()];

		//�ж��Ƿ�����
		if (Cross(csGameData, stNewSnakeHead) || csGameData.GetMap(stNewSnakeHead) > NULL_BLOCK)//ײǽ��Ե�����
		{
			return false;
		}

		//�����ƶ�����
		csGameData.IncTravelDistance();

		//�ж��Ƿ�Ե�ʳ��
		if (csGameData.GetMap(stNewSnakeHead) == FOOD_BLOCK)
		{
			//�ݼ�ʳ����
			csGameData.DecCurrentFoodNum();
			//��������ͷΪԭ����ͷ��ֵ+1�������ƶ���β
			csGameData.GetMap(stNewSnakeHead) = csGameData.GetSnakeLength() + 1;
			//��������ͷλ��
			csGameData.GetSnakeHead() = stNewSnakeHead;
			//�����µ�ʳ��
			if (!csGameData.GetEatAllToProduce() || csGameData.GetCurrentFoodNum() == 0)
			{//������ǳ���ȫ����������ôֱ�����ɣ������жϵ�ǰʳ�����Ƿ�Ϊ0����·��ֵ���ԣ����Ϊfalse�Ų����Ҳ�ֵ��
				ProduceFood(csGameData);
			}
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
				stMove += Game_Data::stSnakeMove[j];//�ƶ�

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