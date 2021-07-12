#include "Game.h"
#include "ThirdFont.h"

void CThirdFont::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CThirdFont::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CThirdFont::Render()
{
	animation_set->at(0)->Render(x, y, -1, 255);
}
