#include "BigWing.h"

CBigWing::CBigWing(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CBigWing::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(30007);
	if (nx < 0) {
		sprite->DrawFlipX(x, y);
	}
	else {
		sprite->Draw(x, y);
	}
}

void CBigWing::Update(DWORD dt)
{
	CGameObject::Update(dt);
}

void CBigWing::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}