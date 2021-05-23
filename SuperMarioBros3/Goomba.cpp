#include "Game.h"
#include "Goomba.h"

CGoomba::CGoomba(float x, float y) :CGameObject(x, y)
{
	this->vx = GOOMBA_SPEED;
};

void CGoomba::Update(DWORD dt)
{
	x += vx * dt;
	/*
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - GOOMBA_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - GOOMBA_WIDTH)
		{
			x = (float)(BackBufferWidth - GOOMBA_WIDTH);
		}
	}
	*/
}

void CGoomba::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(31401);
	else ani = CAnimations::GetInstance()->Get(504);

	ani->Render(x, y);
}