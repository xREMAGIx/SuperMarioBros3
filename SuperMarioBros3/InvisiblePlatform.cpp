#include "InvisiblePlatform.h"


CInvisiblePlatform::CInvisiblePlatform(int width)
{
	this->width = width;
}


void CInvisiblePlatform::Render()
{
}

void CInvisiblePlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + INVISIBLE_PLATFORM_BBOX_HEIGHT;
}

void CInvisiblePlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}