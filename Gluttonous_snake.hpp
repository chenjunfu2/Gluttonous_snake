#pragma once
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <utility>

#include <Windows.h>


//���
#define FOOD_MARK -1//ʳ����
#define SNAKE_TAIL 1//��β���
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
	enum class Move_Direct :long
	{
		Up = 0,
		Down,
		Left,
		Right,
		Arr_Size,
		No_Move = Arr_Size,
	};

private:
	long lMapWidth;//x
	long lMapHigh;//y
	long *lMap;//��ͼ

	long lMoveInterval;//�ƶ����
	My_Point stSnakeHead;//ͷ����
	Move_Direct enMoveDirect;//�ƶ�����
public:

	Snake_Data(long _lMapWidth, long _lMapHigh, long _lMoveInterval, const My_Point &_stSnakeStart, Move_Direct _enDirectStart) :
		lMapWidth(_lMapWidth), lMapHigh(_lMapHigh), lMoveInterval(_lMoveInterval)
	{
		lMap = new long[lMapWidth * lMapHigh];
		Reset(_stSnakeStart, _enDirectStart);
	}

	~Snake_Data(void)
	{
		delete[] lMap;
		lMap = NULL;
	}

	Snake_Data(Snake_Data &&_Move):
		lMapWidth		(_Move.lMapWidth),
		lMapHigh		(_Move.lMapHigh),
		lMap			(_Move.lMap),
		lMoveInterval	(_Move.lMoveInterval),
		stSnakeHead		(std::move(_Move.stSnakeHead)),
		enMoveDirect	(_Move.enMoveDirect)
	{
		_Move.lMapWidth = 0;
		_Move.lMapHigh = 0;
		_Move.lMap = NULL;
		_Move.lMoveInterval = 0;
		_Move.enMoveDirect = Move_Direct::No_Move;
	}

	void Reset(const My_Point &_stSnakeStart, Move_Direct _enDirectStart)
	{
		stSnakeHead = _stSnakeStart;
		enMoveDirect = _enDirectStart;

		memset(lMap, 0, lMapWidth * lMapHigh * sizeof(long));
		GetMap(stSnakeHead) = SNAKE_TAIL;//��ʼ����ͼ����ͷ��λ��
	}

	long& GetMoveInterval(void) 
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

	static constexpr My_Point stSnakeMove[(long)Snake_Data::Move_Direct::Arr_Size] =//�ƶ�����
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

	static Snake_Data::Move_Direct Input(void)
	{
		while (_kbhit() != 0)//����һ������
		{
			switch (_getch())
			{
			case 'w':
			case 'W':
				{
					return Snake_Data::Move_Direct::Up;
				}
				break;
			case 'a':
			case 'A':
				{
					return Snake_Data::Move_Direct::Left;
				}
				break;
			case 's':
			case 'S':
				{
					return Snake_Data::Move_Direct::Down;
				}
				break;
			case 'd':
			case 'D':
				{
					return Snake_Data::Move_Direct::Right;
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

		return Snake_Data::Move_Direct::No_Move;
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

	static void SetOutputFullMode(const Snake_Data &csSnakeData)
	{
		//���û�����Ϊȫ���壬��СΪ��ͼ��С
		setvbuf(stdout, NULL, _IOFBF, (csSnakeData.GetMapHigh() + 2) * (csSnakeData.GetMapWidth() + 2) + 1);
	}

	static void SetOutputLineMode(void)
	{
		setvbuf(stdout, NULL, _IOLBF, BUFSIZ);//�ָ�Ĭ��
	}

	static void ChangeDirect(Snake_Data &csSnakeData, Snake_Data::Move_Direct enMoveDirect)
	{
		//�ж���û�н����ƶ�
		if (enMoveDirect != Snake_Data::Move_Direct::No_Move)
		{
			//�жϸı�ķ����Ƿ�������
			My_Point stCurrent = csSnakeData.GetSnakeHead();
			stCurrent += stSnakeMove[(long)enMoveDirect];
			if (csSnakeData.GetMap(stCurrent) == 0 || csSnakeData.GetMap(stCurrent) + 1 != csSnakeData.GetSnakeLength())//��������
			{
				csSnakeData.GetMoveDirect() = enMoveDirect;//�ı䷽��
			}
		}
	}

	static void ProduceFood(Snake_Data &csSnakeData)//�����޸�������
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
		//�Ƚ���ͷ��ǰ�����ƶ�һ��
		My_Point stNewSnakeHead = csSnakeData.GetSnakeHead();
		stNewSnakeHead += stSnakeMove[(long)csSnakeData.GetMoveDirect()];

		//�ж��Ƿ�����
		if (Cross(csSnakeData, stNewSnakeHead) || csSnakeData.GetMap(stNewSnakeHead) > NULL_BLOCK)//ײǽ��Ե�����
		{
			return false;
		}

		//�ж��Ƿ�Ե�ʳ��
		if (csSnakeData.GetMap(stNewSnakeHead) == FOOD_MARK)
		{
			//��������ͷΪԭ����ͷ��ֵ+1�������ƶ���β
			csSnakeData.GetMap(stNewSnakeHead) = csSnakeData.GetSnakeLength() + 1;
			//�����µ�ʳ��
			ProduceFood(csSnakeData);
		}
		else
		{
			//��������ͷΪԭ����ͷ��ֵ
			csSnakeData.GetMap(stNewSnakeHead) = csSnakeData.GetSnakeLength();
			
			//������������β
			My_Point stCurrent = stNewSnakeHead;//������ֵΪ��β����
			while (csSnakeData.GetMap(stCurrent) != NULL_BLOCK)
			{
				//�鿴�ĸ�������������ֵ��ͬ��ֵ
				for (long j = 0; j < (long)Snake_Data::Move_Direct::Arr_Size; ++j)
				{
					My_Point stMove = stCurrent;
					stMove += stSnakeMove[j];//�ƶ�

					if (!Cross(csSnakeData, stMove) && csSnakeData.GetMap(stMove) == csSnakeData.GetMap(stCurrent))//��ͬ
					{
						--csSnakeData.GetMap(stMove);//�ݼ�
						stCurrent = stMove;//�ƶ�
						break;//������һ��λ��
					}
				}
			}
		}

		//��������ͷλ��
		csSnakeData.GetSnakeHead() = stNewSnakeHead;

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

	static void DrawSnakeTail(void)//������β
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
				long lCurrent = csSnakeData.GetMap({x,y});

				if (lCurrent == NULL_BLOCK)
				{
					DrawNull();
				}
				else if (lCurrent == csSnakeData.GetSnakeLength())
				{
					DrawSnakeHead();
				}
				else if (lCurrent == SNAKE_TAIL)
				{
					DrawSnakeTail();
				}
				else if (lCurrent == FOOD_MARK)
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
	Snake_Data csSnakeData;
public:
	Game(Snake_Data &&_csSnakeData) :
		csSnakeData(std::move(_csSnakeData))
	{
		Control::SetOutputFullMode(csSnakeData);
		//��ʼ�������������
		srand((unsigned int)time(NULL));//�����޸������������
		//�����ʼ��Ϣ
		Draw::InfoDraw();
		//�����������
		Control::GetAnyKey();
	}

	~Game(void)
	{
		Control::SetOutputLineMode();
	}

	Snake_Data &GetSnakeData(void)
	{
		return csSnakeData;
	}

	void Init(void)
	{
		//��ʼ��ʳ��
		Control::ProduceFood(csSnakeData);
	}

	bool Loop(void)
	{
		while (true)
		{
			Control::Wait(csSnakeData.GetMoveInterval());
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