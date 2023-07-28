#include "Gluttonous_snake.hpp"

//地图大小
#define MAP_WIDTH	32//x
#define MAP_HIGH	18//y
//蛇初始坐标
#define INITIAL_X (lGameData[0]/2-1)
#define INITIAL_Y (lGameData[1]/3-1)
//初始移动方向
#define INITIAL_DIRECT Snake_Data::Move_Direct::Down
//初始移动毫秒数
#define MOVE_SPEED 160

int main(int argc, char *argv[])//参数 Gluttonous_snake 1 2 180 0 0 1 崩溃
{
	long lGameData[6]=
	{
		MAP_WIDTH, 
		MAP_HIGH, 
		MOVE_SPEED,
	};

	long lInputData[6];

	for (int i = 1; i < argc && i < 6; ++i)
	{
		if (sscanf(argv[i], "%ld", &lInputData[i - 1]) != 1)
		{
			lInputData[i - 1] = -1;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (lInputData[i] > 0)
		{
			lGameData[i] = lInputData[i];
		}
	}
	
	if (lInputData[3] >= 0 && lInputData[3] < MAP_WIDTH)
	{
		lGameData[3] = lInputData[3];
	}
	else
	{
		lGameData[3] = INITIAL_X;
	}

	if (lInputData[4] >= 0 && lInputData[4] < MAP_HIGH)
	{
		lGameData[4] = lInputData[4];
	}
	else
	{
		lGameData[4] = INITIAL_Y;
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