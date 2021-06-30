#include "EnemyWall.h"


CEnemyWall::CEnemyWall(int height)
{
	this->height = height;
}


void CEnemyWall::Render()
{
	//RenderBoundingBox();
}

void CEnemyWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ENEMY_WALL_BBOX_WIDTH;
	b = y + height;
}