#include "InvisibleBlock.h"


CInvisibleBlock::CInvisibleBlock(int width)
{
	this->width = width;
}


void CInvisibleBlock::Render()
{
	//RenderBoundingBox();
}

void CInvisibleBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + INVISIBLE_BLOCK_BBOX_HEIGHT;
}
