#pragma once

#include <stdio.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>

class Game_Init
{
public:
	static void Global_Init(void)//����IO�ַ���
	{
		(void)_setmode(_fileno(stdout), _O_U16TEXT);//��������ַ���
		(void)_setmode(_fileno(stdin), _O_U16TEXT);//���������ַ���
	}
};