#include "InvisiblePlatform.h"


CInvisiblePlatform::CInvisiblePlatform(int width)
{
	this->width = width;
}


void CInvisiblePlatform::Render()
{
	//RenderBoundingBox();
}

void CInvisiblePlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + INVISIBLE_PLATFORM_BBOX_HEIGHT;
}
