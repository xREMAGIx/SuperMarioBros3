#include "ThirdFont.h"

CThirdFont::CThirdFont(float x, float y) :CGameObject(x, y)
{
}

void CThirdFont::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_THIRD_FONT)->Render(x, y);

	//RenderBoundingBox();
}

void CThirdFont::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}


void CThirdFont::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CThirdFont::SetState(int state)
{
	CGameObject::SetState(state);
}
