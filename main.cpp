#include "Play_Game.hpp"

//地图默认大小
#define DEFAULT_MAP_WIDTH	32//x
#define DEFAULT_MAP_HIGH	18//y
//初始移动毫秒数
#define DEFAULT_MOVE_INTERVAL 160
//初始蛇头位置
#define DEFAULT_BEG_X		0
#define DEFAULT_BEG_Y		0
//初始蛇移动方向
#define DEFAULT_MOVE_DIRECT Game_Data::Move_Direct::Right


int main(int argc, char *argv[])
{
	/*
	启动命令格式
	[地图宽] [地图高] [移动间隔] [蛇头初始坐标X] [蛇头初始坐标y] [初始移动方向]

	参数可以省略，省略的部分自动使用默认参数，多余、错误参数将被忽略，
	[蛇头初始坐标X] [蛇头初始坐标y]默认使用[地图宽] [地图高]的二分之一
	[初始移动方向]取值为0 1 2 3分别对应上 下 左 右
	*/

	enum :long
	{
		arr_beg = 0,
		map_width = arr_beg,
		map_high,
		mov_intertval,
		beg_x,
		beg_y,
		beg_direct,
		arr_end,
	};

	long lGameData[arr_end] =
	{
		DEFAULT_MAP_WIDTH,
		DEFAULT_MAP_HIGH,
		DEFAULT_MOVE_INTERVAL,
		DEFAULT_BEG_X,
		DEFAULT_BEG_Y,
		(long)DEFAULT_MOVE_DIRECT,
	};

	long lInputData[arr_end];
	memset(lInputData, -1, sizeof(lInputData));

	//转换命令参数
	for (int i = arr_beg; i < arr_end && i < argc; ++i)
	{
		(void)sscanf(argv[i + 1], "%ld", &lInputData[i]);
	}

	//校验命令参数
	for (int i = map_width; i <= mov_intertval; ++i)
	{
		if (lInputData[i] > 0)
		{
			lGameData[i] = lInputData[i];
		}
	}
	
	if (lInputData[beg_x] >= 0 && lInputData[beg_x] < lGameData[map_width])
	{
		lGameData[beg_x] = lInputData[beg_x];
	}

	if (lInputData[beg_y] >= 0 && lInputData[beg_y] < lGameData[map_high])
	{
		lGameData[beg_y] = lInputData[beg_y];
	}

	if (lInputData[beg_direct] >= (long)Game_Data::Move_Direct::Arr_Beg &&
		lInputData[beg_direct] < (long)Game_Data::Move_Direct::Arr_End)
	{
		lGameData[beg_direct] = lInputData[beg_direct];
	}

	//开始游戏
	Play_Game csGame(Game_Data{lGameData[map_width],lGameData[map_high],lGameData[mov_intertval]});
	do
	{
		csGame.Init(My_Point{lGameData[beg_x],lGameData[beg_y]}, (Game_Data::Move_Direct)lGameData[beg_direct]);
	}while (csGame.Loop());

	return 0;
}