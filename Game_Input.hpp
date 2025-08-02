#pragma once
#include "Game_Control.hpp"

class Game_Input
{
public:
	static Game_Data::Move_Direct Input(void)
	{
		while (_kbhit() != 0)//存在一个输入
		{
			int iInput = _getwch();
			if (iInput == 0xE0 || iInput == 0x00)//转义码
			{
				int iNewInput = _getwch();//重读
				iInput = ((int)((unsigned char)iNewInput)) | (((int)((unsigned char)iInput + 0x01)) << 8);
			}

			switch (iInput)
			{
			case L'w':
			case L'W':
			case 0xE148:
				{
					return Game_Data::Move_Direct::Up;
				}
				break;
			case L'a':
			case L'A':
			case 0xE14B:
				{
					return Game_Data::Move_Direct::Left;
				}
				break;
			case L's':
			case L'S':
			case 0xE150:
				{
					return Game_Data::Move_Direct::Down;
				}
				break;
			case L'd':
			case L'D':
			case 0xE14D:
				{
					return Game_Data::Move_Direct::Right;
				}
				break;
			case L'p'://暂停
			case L'P':
				{
					int iGet;
					do
					{
						iGet = _getwch();
					} while (iGet != L'p' && iGet != L'P');
					Game_Control::Wait(50);//给50ms反应时间
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
};