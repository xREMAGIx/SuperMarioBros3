#include "Cloud.h"

void CCloud::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20007);
	sprite->Draw(x, y);
}

void CCloud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CLOUD_BBOX_WIDTH;
	b = y + CLOUD_BBOX_HEIGHT;
}