#include "Game.h"
#include "SuperLeaf.h"

CSuperLeaf::CSuperLeaf(float x, float y) :CGameObject(x, y)
{
	this->vx = SUPER_LEAF_SPEED_X;
	this->vy = SUPER_LEAF_SPEED_Y;
};

void CSuperLeaf::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	/*
	
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 100 || x >= 150) {
		vx = -vx;
		if (x <= 100)
		{
			x = 100;
		}
		else if (x >= 150)
		{
			x = (float)(150);
		}
	}

	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	if (y <= 0 || y >= BackBufferHeight - SUPER_LEAF_WIDTH) {
		vy = -vy;
		if (y <= 0)
		{
			y = 0;
		}
		else if (y >= BackBufferHeight - SUPER_LEAF_HEIGHT)
		{
			y = (float)(BackBufferHeight - SUPER_LEAF_HEIGHT);
		}
	}
	*/
}

void CSuperLeaf::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20002);
	if (vx < 0) {
		sprite->DrawFlipX(x, y);
	}
	else {
		sprite->Draw(x, y);
	}
	

}