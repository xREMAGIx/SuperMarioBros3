#include "Game.h"
#include "TreeWorld.h"

void CTreeWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TREE_WORLD_WIDTH;
	bottom = y + TREE_WORLD_HEIGHT;
}

void CTreeWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CTreeWorld::Render()
{
	animation_set->at(0)->Render(x, y, -1, 255);
}
