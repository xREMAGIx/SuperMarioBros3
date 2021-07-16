#include "Game.h"
#include "FireBall.h"


CFireball::CFireball()
{
	SetState(COIN_STATE_IDLE);
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_WIDTH;
	bottom = y + FIREBALL_HEIGHT;
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void CFireball::Render()
{
}

void CFireball::SetState(int state)
{
	CGameObject::SetState(state);
}
