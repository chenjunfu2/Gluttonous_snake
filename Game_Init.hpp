#pragma once

#include <stdio.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>

class Game_Init
{
public:
	static void Global_Init(void)//设置IO字符集
	{
		(void)_setmode(_fileno(stdout), _O_U16TEXT);//设置输出字符集
		(void)_setmode(_fileno(stdin), _O_U16TEXT);//设置输入字符集
	}
};