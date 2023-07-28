#include "Gluttonous_snake.hpp"


//地图大小
#define MAP_WIDTH	24//x
#define MAP_HIGH	16//y

//蛇初始坐标
#define INITIAL_X (MAP_WIDTH/2)
#define INITIAL_Y (MAP_HIGH/4)
//初始移动方向
#define INITIAL_DIRECT Snake_Data::Move_Direct::Down
//初始移动毫秒数
#define MOVE_SPEED 180

int main(void)
{
	Snake_Data csSnakeData(MAP_WIDTH, MAP_HIGH, {INITIAL_X,INITIAL_Y}, INITIAL_DIRECT);
	Game csGame(csSnakeData, MOVE_SPEED);
	csGame.Init();

	while (csGame.Loop())
	{
		csSnakeData.Reset({INITIAL_X,INITIAL_Y}, INITIAL_DIRECT);
		csGame.Init();
		Draw::Clear();
	}

	return 0;
}