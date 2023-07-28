#include "Gluttonous_snake.hpp"

//地图默认大小
#define MAP_WIDTH	32//x
#define MAP_HIGH	18//y
//初始移动毫秒数
#define MOVE_INTERVAL 160
//初始移动方向
#define INITIAL_DIRECT Snake_Data::Move_Direct::Down

int main(int argc, char *argv[])
{
	/*
	启动命令格式
	[地图宽] [地图高] [移动间隔] [蛇头初始坐标X] [蛇头初始坐标y] [初始移动方向]

	参数可以省略，省略的部分自动使用默认参数，多余、错误参数将被忽略，
	[蛇头初始坐标X] [蛇头初始坐标y]默认使用[地图宽] [地图高]的二分之一
	[初始移动方向]取值为0 1 2 3分别对应上 下 左 右
	*/

	long lGameData[6]=
	{
		MAP_WIDTH, 
		MAP_HIGH, 
		MOVE_INTERVAL,
	};

	long lInputData[6];
	memset(lInputData, -1, sizeof(lInputData));

	for (int i = 1; i < argc && (i - 1) < 6; ++i)
	{
		(void)sscanf(argv[i], "%ld", &lInputData[i - 1]);
	}

	for (int i = 0; i < 3; ++i)
	{
		if (lInputData[i] > 0)
		{
			lGameData[i] = lInputData[i];
		}
	}
	
	if (lInputData[3] >= 0 && lInputData[3] < lGameData[0])
	{
		lGameData[3] = lInputData[3];
	}
	else
	{
		lGameData[3] = lGameData[0] / 2;
	}

	if (lInputData[4] >= 0 && lInputData[4] < lGameData[1])
	{
		lGameData[4] = lInputData[4];
	}
	else
	{
		lGameData[4] = lGameData[1] / 2;
	}

	if (lInputData[5] >= 0 && lInputData[5] < (long)Snake_Data::Move_Direct::Arr_Size)
	{
		lGameData[5] = lInputData[5];
	}
	else
	{
		lGameData[5] = (long)INITIAL_DIRECT;
	}

	Game csGame(Snake_Data{lGameData[0],lGameData[1],lGameData[2],My_Point{lGameData[3],lGameData[4]}, (Snake_Data::Move_Direct)lGameData[5]});

	csGame.Init();
	while (csGame.Loop())
	{
		csGame.GetSnakeData().Reset(My_Point{lGameData[3],lGameData[4]}, (Snake_Data::Move_Direct)lGameData[5]);
		csGame.Init();
		Draw::Clear();
	}

	return 0;
}