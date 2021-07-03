#include "SmallWing.h"

CSmallWing::CSmallWing(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CSmallWing::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(30008);
	if (nx < 0) {
		sprite->DrawFlipX(x, y);
	}
	else {
		sprite->Draw(x, y);
	}
}

void CSmallWing::Update(DWORD dt)
{
	CGameObject::Update(dt);
}

void CSmallWing::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}