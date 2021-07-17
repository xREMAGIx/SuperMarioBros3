#include "Game.h"
#include "TreeWorld.h"

void CTreeWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CTreeWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CTreeWorld::Render()
{
	animation_set->at(0)->Render(x, y, -1, 255);
}
