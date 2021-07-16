#include "Game.h"
#include "Boomerang.h"


CBoomerang::CBoomerang()
{
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void CBoomerang::Render()
{
}

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
}
