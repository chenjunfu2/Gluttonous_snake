#include "Game_Play.hpp"
#include <time.h>

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
//初始最大食物个数
#define DEFAULT_FOOD_MAX_NUM 1
//初始获胜长度
#define DEFAULT_WIN_LENGTH -1//填满整个地图
//初始随机种子
#define DEFAULT_RANDOM_SEED -1//每次开局都是用随机种子


int main(int argc, char *argv[])
{
	/*
	启动命令格式:
	[地图宽] [地图高] [移动间隔] [蛇头初始坐标X] [蛇头初始坐标y] [初始移动方向] [最大食物数] [获胜长度] [随机数种子]

	备注:
	参数可以省略，省略的部分自动使用默认参数，多余、错误参数将被默认参数取代
	[地图宽]			默认值32		不得小等于0			地图的宽度
	[地图高]			默认值18		不得小等于0			地图的高度
	[移动间隔]		默认值160	不得小等于0			代表蛇每次自动爬行之间间隔的毫秒数
	[蛇头初始坐标X]	默认值0		不得超出地图边界		小于0或大于等于[地图宽]则超出边界
	[蛇头初始坐标y]	默认值0		不得超出地图边界		小于0或大于等于[地图高]则超出边界
	[初始移动方向]	默认值3		取值只能为0 1 2 3	分别对应上 下 左 右
	[最大食物数]		默认值1		不得小于0			只代表能同时出现的最大食物数，实际食物数可能小于最大食物数
	[获胜长度]		默认值-1		不得小于0(-1除外)		即蛇长度大于等于其时玩家获胜，如果为-1则蛇必须填满地图玩家才能获胜
	[随机数种子]		默认值-1		可以为任意值			如果为-1则每次随机开局，否则每次都是用固定随机数种子开局（即食物随机位置固定）

	示例参数:
	16 8 220 0 0 3 2 32 1
	地图宽高为16*8
	移动间隔为220ms
	蛇头初始位置为(0,0)且方向向右
	同时出现的最大食物数为2
	蛇长度达到32时即获胜
	每次都是用1作为随机数种子，食物位置每局固定
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
		food_max_num,
		win_length,
		random_seed,
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
		DEFAULT_FOOD_MAX_NUM,
		DEFAULT_WIN_LENGTH,
		DEFAULT_RANDOM_SEED,
	};

	{//限制lInputData作用域
		long lInputData[arr_end];
		memset(lInputData, -1, sizeof(lInputData));

		//转换命令参数
		for (int i = arr_beg; i < arr_end && (i + 1) < argc; ++i)
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

		if (lInputData[food_max_num] >= 0)
		{
			lGameData[food_max_num] = lInputData[food_max_num];
		}

		if (lInputData[win_length] >= 0)//小于0则填满整个地图获胜
		{
			lGameData[win_length] = lInputData[win_length];
		}

		if (lInputData[random_seed] != -1)
		{
			lGameData[random_seed] = lInputData[random_seed];//之后强制转换为unsigned int
		}
	}

	//开始游戏
	Game_Play csGame(
		Game_Data
		{
			lGameData[map_width],
			lGameData[map_high],
			lGameData[mov_intertval],
			lGameData[win_length],
			lGameData[food_max_num],
		},
		lGameData[random_seed] == -1 ? (unsigned int)time(NULL) : (unsigned int)lGameData[random_seed], 
		My_Point
		{
			lGameData[beg_x],
			lGameData[beg_y],
		}, 
		(Game_Data::Move_Direct)lGameData[beg_direct]);
	do
	{
		csGame.Init();
		if (lGameData[random_seed] != -1)//固定种子，每局重置
		{
			csGame.SetRandomSeed((unsigned int)lGameData[random_seed]);
		}
	}while (csGame.Loop());

	return 0;
}