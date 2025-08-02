#include "Game_Init.hpp"
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
//初始食物生成方式
#define DEFAULT_FOOD_PRODUCE true//吃完所有再生成

int wmain(int argc, wchar_t *argv[])
{
	/*
	启动命令格式:
	[地图宽] [地图高] [移动间隔] [蛇头初始坐标X] [蛇头初始坐标y] [初始移动方向] [最大食物数] [获胜长度] [随机数种子] [食物生成方式]

	备注:
	参数可以省略，省略的部分自动使用默认参数，多余、错误参数将被默认参数取代
	[地图宽]			默认值32		不得小于2			地图的宽度
	[地图高]			默认值18		不得小于2			地图的高度
	[移动间隔]		默认值160	不得小于0			代表蛇每次自动爬行之间间隔的毫秒数
	[蛇头初始坐标X]	默认值0		不得超出地图边界		小于0或大于等于[地图宽]则超出边界
	[蛇头初始坐标y]	默认值0		不得超出地图边界		小于0或大于等于[地图高]则超出边界
	[初始移动方向]	默认值4		取值只能为1 2 3 4	分别对应上 下 左 右
	[最大食物数]		默认值1		不得小于0			只代表能同时出现的最大食物数，实际食物数可能小于最大食物数
	[获胜长度]		默认值-1		不得小于0(-1除外)		即蛇长度大于等于其时玩家获胜，如果为-1则蛇必须填满地图玩家才能获胜
	[随机数种子]		默认值-1		可以为任意值			如果为-1则每次随机开局，否则每次都是用固定随机数种子开局（即食物随机位置固定）
	[食物生成方式]	默认值1		取值只能为0 1		分别对应false true，如果为true则吃完所有食物再生成下一波，否则吃一个生成一个

	示例参数:
	16 8 220 0 0 2 2 32 1 0
	地图宽高为16*8
	移动间隔为220ms
	蛇头初始位置为(0,0)且方向向下
	同时出现的最大食物数为2
	蛇长度达到32时即获胜
	每次都是用1作为随机数种子，食物位置每局固定
	每吃完一个食物生成一个
	*/

	//全局初始化
	Game_Init::Global_Init();

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
		food_produce,

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
		DEFAULT_FOOD_PRODUCE,
	};

	struct{long lMin, lMax;}//min和max都能取到
	lInputRange[arr_end] =
	{
		{2,LONG_MAX},//map_width
		{2,LONG_MAX},//map_high

		{0,LONG_MAX},//mov_intertval

		{0,LONG_MAX},//beg_x
		{0,LONG_MAX},//beg_y
		{(long)Game_Data::Move_Direct::Beg + 1,(long)Game_Data::Move_Direct::End - 1},//beg_direct

		{0,LONG_MAX},//food_max_num
		{0,LONG_MAX},//win_length 小于0则填满整个地图获胜

		{LONG_MIN,LONG_MAX},//random_seed
		{0,1},//food_produce true or false
	};

	//转换命令参数
	for (int i = arr_beg; i < arr_end && i < argc - 1; ++i)
	{
		long lInput;
		if (swscanf(argv[i + 1], L"%ld", &lInput) != 1)
		{
			continue;
		}

		if (lInput >= lInputRange[i].lMin && lInput <= lInputRange[i].lMax)
		{
			lGameData[i] = lInput;
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
			(unsigned int)(lGameData[random_seed] != -1 ? lGameData[random_seed] : time(NULL)),
			(bool)lGameData[food_produce],
		},
		My_Point
		{
			lGameData[beg_x],
			lGameData[beg_y],
		}, 
		(Game_Data::Move_Direct)lGameData[beg_direct]);
	do
	{
		if (lGameData[random_seed] != -1)//固定种子，每局重置
		{
			csGame.SetRandomSeed((unsigned int)lGameData[random_seed]);
		}
		csGame.Init();
	}while (csGame.Loop());

	return 0;
}