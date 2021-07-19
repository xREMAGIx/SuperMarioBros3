#include "SmallWing.h"

CSmallWing::CSmallWing(float x, float y)
{
	this->x = x;
	this->y = y;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SMALLWING_ANI_SET_ID);
	SetAnimationSet(ani_set);
	SetState(SMALLWING_ANI_IDLE);
}

void CSmallWing::Render()
{
	int ani = SMALLWING_ANI_IDLE;

	if (state == SMALLWING_STATE_FLYING) {
		ani = SMALLWING_ANI_FLYING;
	}

	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, nx, 255);
	}
}

void CSmallWing::Update(DWORD dt)
{
	CGameObject::Update(dt);
}

void CSmallWing::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CSmallWing::SetState(int state)
{
	CGameObject::SetState(state);
}