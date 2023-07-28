#pragma once
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#include <Windows.h>


//���
#define FOOD_MARK -1//ʳ����
#define SNAKE_HEAD 1//��ͷ���
#define NULL_BLOCK 0//�յر��

struct My_Point
{
	long x, y;

public:
	My_Point &operator+=(const My_Point &_Other)
	{
		x += _Other.x;
		y += _Other.y;
		return *this;
	}
};


class Snake_Data
{
public:
	enum Move_Direct :long
	{
		Up = 0,
		Down,
		Left,
		Right,
		Arr_Size,
		No_Move = Arr_Size,
	};

private:
	const long lMapWidth;//x
	const long lMapHigh;//y
	long *lMap;//��ͼ

	long lSnakeLength;//�߳���
	My_Point stSnakeHead;//ͷ����
	Move_Direct enMoveDirect;//�ƶ�����
public:

	Snake_Data(long _lMapWidth, long _lMapHigh, const My_Point &_stSnakeStart, Move_Direct _enDirectStart) :
		lMapWidth(_lMapWidth), lMapHigh(_lMapHigh)
	{
		lMap = new long[lMapWidth * lMapHigh];
		Reset(_stSnakeStart, _enDirectStart);
	}

	~Snake_Data(void)
	{
		delete[] lMap;
		lMap = NULL;
	}

	void Reset(const My_Point &_stSnakeStart, Move_Direct _enDirectStart)
	{
		stSnakeHead = _stSnakeStart;
		enMoveDirect = _enDirectStart;
		lSnakeLength = 1;//�߳�����Ϊ1

		memset(lMap, 0, lMapWidth * lMapHigh * sizeof(long));
		GetMap(stSnakeHead) = SNAKE_HEAD;//��ʼ����ͼ����ͷ��λ��
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

class Control
{
private:
	static bool Cross(const Snake_Data &csSnakeData, const My_Point &stNewSnakeHead)
	{
		//Խ��
		if (stNewSnakeHead.x >= csSnakeData.GetMapWidth() ||
			stNewSnakeHead.y >= csSnakeData.GetMapHigh() ||
			stNewSnakeHead.x < 0 ||
			stNewSnakeHead.y < 0)
		{
			return true;
		}
		return false;
	}

	static constexpr My_Point stSnakeMove[(long)Snake_Data::Arr_Size] =//�ƶ�����
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

	static bool Win(const Snake_Data &csSnakeData)
	{
		if (csSnakeData.GetSnakeLength() == csSnakeData.GetMapHigh() * csSnakeData.GetMapWidth())
		{
			return true;
		}
		return false;
	}

	static bool Lose(const Snake_Data &csSnakeData, const My_Point &stNewSnakeHead)
	{
		//Խ�����������
		if (Cross(csSnakeData, stNewSnakeHead) || csSnakeData.GetMap(stNewSnakeHead) > NULL_BLOCK)
		{
			return true;
		}

		return false;
	}

	static Snake_Data::Move_Direct Input(void)
	{
		while (_kbhit() != 0)//����һ������
		{
			switch (_getch())
			{
			case 'w':
			case 'W':
				{
					return Snake_Data::Up;
				}
				break;
			case 'a':
			case 'A':
				{
					return Snake_Data::Left;
				}
				break;
			case 's':
			case 'S':
				{
					return Snake_Data::Down;
				}
				break;
			case 'd':
			case 'D':
				{
					return Snake_Data::Right;
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
				}
				continue;
			default:
				continue;
			}
		}

		return Snake_Data::No_Move;
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

	static void ChangeDirect(Snake_Data &csSnakeData, Snake_Data::Move_Direct enMoveDirect)
	{
		//�ж���û�н����ƶ�
		if (enMoveDirect != Snake_Data::No_Move)
		{
			//�жϸı�ķ����Ƿ�������
			My_Point stCurrent = csSnakeData.GetSnakeHead();
			stCurrent += stSnakeMove[(long)enMoveDirect];
			if (csSnakeData.GetMap(stCurrent) != SNAKE_HEAD + 1)//��������
			{
				csSnakeData.GetMoveDirect() = enMoveDirect;//�ı䷽��
			}
		}
	}

	static void ProduceFood(Snake_Data &csSnakeData)
	{
		if (csSnakeData.GetSnakeLength() >= csSnakeData.GetMapHigh() * csSnakeData.GetMapWidth())
		{
			return;//����ʧ��
		}

		My_Point stFood;
		do
		{
			stFood.x = rand() % csSnakeData.GetMapWidth();
			stFood.y = rand() % csSnakeData.GetMapHigh();
		} while (csSnakeData.GetMap(stFood) != 0);//�����յ�Ϊֹ

		csSnakeData.GetMap(stFood) = FOOD_MARK;
	}

	static bool Move(Snake_Data &csSnakeData)
	{
		//�Ƚ���ͷ��ǰ�ƶ�һ��
		My_Point stNewSnakeHead = csSnakeData.GetSnakeHead();
		stNewSnakeHead += stSnakeMove[(long)csSnakeData.GetMoveDirect()];

		//�ж��Ƿ�����
		if (Lose(csSnakeData, stNewSnakeHead))
		{
			return false;
		}

		//������ͷλ��
		csSnakeData.GetSnakeHead() = stNewSnakeHead;

		//�ж��Ƿ�Ե�ʳ��
		bool bEatFood = false;
		if (csSnakeData.GetMap(stNewSnakeHead) == FOOD_MARK)
		{
			++csSnakeData.GetSnakeLength();//�����ߵĳ���
			ProduceFood(csSnakeData);//�����µ�ʳ��
			bEatFood = true;
		}

		//��������ͷ
		csSnakeData.GetMap(stNewSnakeHead) = SNAKE_HEAD;

		//��������
		My_Point stCurrent = stNewSnakeHead;//������ֵΪ��β����
		for (long i = 0; i < csSnakeData.GetSnakeLength(); ++i)
		{
			//�鿴�ĸ�������������ֵ��ͬ��ֵ
			for (long j = 0; j < (long)Snake_Data::Arr_Size; ++j)
			{
				My_Point stMove = stCurrent;
				stMove += stSnakeMove[j];//�ƶ�

				if (!Cross(csSnakeData, stMove) && csSnakeData.GetMap(stMove) == csSnakeData.GetMap(stCurrent))//��ͬ
				{
					++csSnakeData.GetMap(stMove);//����
					stCurrent = stMove;//�ƶ�
				}
			}
		}

		//������β
		if (!bEatFood)
		{
			csSnakeData.GetMap(stCurrent) = NULL_BLOCK;//������βΪ��
		}

		return true;
	}
};

class Draw
{
private:
	//����
	static void DrawBorder(void)//���Ʊ߽�
	{
		fputs("��", stdout);
	}

	static void DrawNull(void)//���ƿյ�
	{
		fputs("  ", stdout);
	}

	static void DrawFood(void)//����ʳ��
	{
		fputs("��", stdout);
	}

	static void DrawSnakeHead(void)//������ͷ
	{
		fputs("��", stdout);
	}

	static void DrawSnakeBody(void)//��������
	{
		fputs("��", stdout);
	}

	static void RewindOutput(void)//�ص���ͷ
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
	}

	static void NewLine(void)//����
	{
		putchar('\n');
	}

	static void HideCursor(void)//���ع��
	{
		CONSOLE_CURSOR_INFO info = {1,0};
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}

	static void Flush(void)//ˢ�����
	{
		fflush(stdout);
	}

	static void DrawLineBorder(const Snake_Data &csSnakeData)//�����б߽�
	{
		for (long x = 0; x < csSnakeData.GetMapWidth() + 2; ++x)//+2����Ϊ�����Ҳ����1��߽�
		{
			DrawBorder(); 
		}
		NewLine();
	}

public:
	static void InfoDraw(void)
	{
		printf("---̰����---\nW-��  A-��\nS-��  D-��\nP-��ͣ/����\n------------\n\n���������ʼ...");
		Flush();
	}

	static void SnakeDraw(const Snake_Data &csSnakeData)
	{
		//���ع�겢��λ����ͷ
		HideCursor();
		RewindOutput();

		//�����ϱ߽�
		DrawLineBorder(csSnakeData);

		for (long y = 0; y < csSnakeData.GetMapHigh(); ++y)
		{
			DrawBorder();//������߽�
			for (long x = 0; x < csSnakeData.GetMapWidth(); ++x)
			{
				if (csSnakeData.GetMap({x,y}) == NULL_BLOCK)
				{
					DrawNull();
				}
				else if (csSnakeData.GetMap({x,y}) == SNAKE_HEAD)
				{
					DrawSnakeHead();
				}
				else if (csSnakeData.GetMap({x,y}) == FOOD_MARK)
				{
					DrawFood();
				}
				else//����
				{
					DrawSnakeBody();
				}
			}
			DrawBorder();//�����ұ߽�
			NewLine();//����һ�л���
		}

		//�����±߽�
		DrawLineBorder(csSnakeData);

		printf("����:%ld\n", csSnakeData.GetSnakeLength());

		//ˢ��
		Flush();
	}

	static void LoseDraw(void)
	{
		printf("\n�����ˣ�");
		Flush();
	}

	static void WinDraw(void)
	{
		printf("\n��Ӯ�ˣ�");
		Flush();
	}

	static void EndGame(const char *pNew, const char *pLeave)
	{
		printf("����%s�¿�һ�֣�����%s������Ϸ", pNew, pLeave);
		Flush();
	}

	static void Clear(void)//����
	{
		system("cls");
	}
};




class Game
{
private:
	Snake_Data &csSnakeData;
	long lLoopSpeed;
public:
	Game(Snake_Data &_csSnakeData, long _lLoopSpeed) :
		csSnakeData(_csSnakeData), lLoopSpeed(_lLoopSpeed)
	{
		//���û�����Ϊȫ���壬��СΪ��ͼ��С
		setvbuf(stdout, NULL, _IOFBF, csSnakeData.GetMapHigh() * csSnakeData.GetMapWidth() + 1);
		//��ʼ�������������
		srand((unsigned int)time(NULL));
		//�����ʼ��Ϣ
		Draw::InfoDraw();
		//�����������
		Control::GetAnyKey();
	}

	void Init(void)
	{
		//��ʼ��ʳ��
		Control::ProduceFood(csSnakeData);
	}

	void ChangeLoopSpeed(long lNewLoopSpeed)
	{
		lLoopSpeed = lNewLoopSpeed;
	}

	long GetLoopSpeed(void) const
	{
		return lLoopSpeed;
	}

	bool Loop(void)
	{
		while (true)
		{
			Control::Wait(lLoopSpeed);
			Control::ChangeDirect(csSnakeData, Control::Input());

			if (!Control::Move(csSnakeData))
			{
				Draw::SnakeDraw(csSnakeData);
				Draw::LoseDraw();
				break;
			}

			Draw::SnakeDraw(csSnakeData);

			if (Control::Win(csSnakeData))
			{
				Draw::WinDraw();
				break;
			}
		}

		Draw::EndGame("H", "L");
		return Control::SwitchInput("Hh", 2, "Ll", 2);
	}
};


/*
\033[��ʾ��ʽ��ǰ��ɫ������ɫm

ANSI������ :
\033[0m   �ر���������
\033[1m   ���ø�����
\033[4m   �»���
\033[5m   ��˸
\033[7m   ����
\033[8m   ����
\033[30m   --   \033[37m   ����ǰ��ɫ
\033[40m   --   \033[47m   ���ñ���ɫ
\033[nA   �������n��
\033[nB   �������n��
\033[nC   �������n��
\033[nD   �������n��
\033[y;xH ���ù��λ��
\033[2J   ����
\033[K   ����ӹ�굽��β������
\033[s   ������λ��
\033[u   �ָ����λ��
\033[?25l   ���ع��
\033[?25h   ��ʾ���

����
\033[2J\033[0;0H  �����ҽ�����ö�
\033[1;31mhello world\033[0m  ʹ�ø�����ɫ���hello world���ָ���ɫΪNONE


��ɫ��Ϊ����ɫ������ɫ��30~39������������ɫ��40~49���ñ�����

����ɫ                        ����ɫ
40: ��                          30: ��
41: ��                          31: ��
42: ��                          32: ��
43: ��                          33: ��
44: ��                          34: ��
45: ��                          35: ��
46: ����                        36: ����
47: ��ɫ                        37: ��ɫ

�ǵ��ڴ�ӡ��֮�󣬰���ɫ�ָ���NONE����Ȼ�ٺ���Ĵ�ӡ������ű�ɫ��

����һЩת���ַ�
\a alarm�����ӣ�������
\b backspace (�˸��)
\f form feed����ҳ����
\t Tab��Tab������
\v Vertical Tab��ˮƽ�Ʊ�
\n newline�����У�
\r carriage return���س���
*/