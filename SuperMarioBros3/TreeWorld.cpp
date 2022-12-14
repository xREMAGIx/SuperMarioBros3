#include "Game.h"
#include "TreeWorld.h"

CTreeWorld::CTreeWorld(float x, float y) :CGameObject(x, y)
{
}

void CTreeWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CTreeWorld::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CTreeWorld::Render()
{
	int aniId = ID_ANI_TREE_WORLD;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
