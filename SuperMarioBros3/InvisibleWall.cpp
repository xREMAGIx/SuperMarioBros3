#include "InvisibleWall.h"


CInvisibleWall::CInvisibleWall(int height)
{
	this->height = height;
}


void CInvisibleWall::Render()
{
}

void CInvisibleWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + INVISIBLE_BLOCK_BBOX_WIDTH;
	b = y + height;
}