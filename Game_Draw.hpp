#pragma once

#include "Game_Data.hpp"
#include "Game_Sign.hpp"

#include <stdio.h>
#include <Windows.h>

class Game_Draw
{
private:
	//����
	static void BorderBlock(void)//���Ʊ߽�
	{
		fputs("��", stdout);
	}

	static void NullBlock(void)//���ƿյ�
	{
		fputs("  ", stdout);
	}

	static void FoodBlock(void)//����ʳ��
	{
		fputs("��", stdout);
	}

	static void SnakeHead(void)//������ͷ
	{
		fputs("��", stdout);
	}

	static void SnakeBody(void)//��������
	{
		fputs("��", stdout);
	}

	static void SnakeTail(void)//������β
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

	static void LineBorderBlock(const Game_Data &csGameData)//�����б߽�
	{
		for (long x = 0; x < csGameData.GetMapWidth() + 2; ++x)//+2����Ϊ�����Ҳ����1��߽�
		{
			BorderBlock();
		}
		NewLine();
	}

public:
	static void Info(void)
	{
		printf("---̰����---\nW--��  A--��\nS--��  D--��\n\nP--��ͣ/����\n------------\n\n���������ʼ...");
		Flush();
	}

	static void Draw(const Game_Data &csGameData)
	{
		//���ع�겢��λ����ͷ
		HideCursor();
		RewindOutput();

		//�����ϱ߽�
		LineBorderBlock(csGameData);

		for (long y = 0; y < csGameData.GetMapHigh(); ++y)
		{
			BorderBlock();//������߽�
			for (long x = 0; x < csGameData.GetMapWidth(); ++x)
			{
				long lCurrent = csGameData.GetMap({x,y});

				if (lCurrent == NULL_BLOCK)
				{
					NullBlock();
				}
				else if (lCurrent == csGameData.GetSnakeLength())//�����ֻ��1��ͷ��β����ͬ�ģ�����ͷ�����Ȼ���
				{
					SnakeHead();
				}
				else if (lCurrent == SNAKE_TAIL)
				{
					SnakeTail();
				}
				else if (lCurrent == FOOD_BLOCK)
				{
					FoodBlock();
				}
				else//����
				{
					SnakeBody();
				}
			}
			BorderBlock();//�����ұ߽�
			NewLine();//����һ�л���
		}

		//�����±߽�
		LineBorderBlock(csGameData);

		printf("����:%ld\n", csGameData.GetSnakeLength());

		//ˢ��
		Flush();
	}

	static void Lose(void)
	{
		printf("\n�����ˣ�\n");
		Flush();
	}

	static void Win(void)
	{
		printf("\n��Ӯ�ˣ�\n");
		Flush();
	}

	static void End(const char *pNew, const char *pLeave)
	{
		printf("����%s�¿�һ��\n����%s������Ϸ", pNew, pLeave);
		Flush();
	}

	static void Clear(void)//����
	{
		system("cls");
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