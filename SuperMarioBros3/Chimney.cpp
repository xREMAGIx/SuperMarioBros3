#include "Chimney.h"


CChimney::CChimney(int width, int height)
{
	this->width = width;
	this->height = height;
}


void CChimney::Render()
{
	//RenderBoundingBox();
}

void CChimney::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}