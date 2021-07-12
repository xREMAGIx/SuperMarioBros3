#include <algorithm>
#include "debug.h"

#include "Game.h"
#include "MarioWorld.h"


void CMarioWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CMarioWorld::Render()
{
	animation_set->at(0)->Render(x, y, nx, 255);
}

void CMarioWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MARIO_SMALL_BBOX_WIDTH;
	bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}

void CMarioWorld::SetState(int state)
{
	CGameObject::SetState(state);
}