#include "Game.h"
#include "Boomerang.h"


CBoomerang::CBoomerang()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET_ID);
	SetAnimationSet(ani_set);
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
	int ani = BOOMERANG_ANI_IDLE;
	if (state == BOOMERANG_STATE_THROWN) {
		ani = BOOMERANG_ANI_THROWN;
	}
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, -nx, 255);
	}
}

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_STATE_IDLE: {
		vx = 0;
		break;
	}
	case BOOMERANG_STATE_THROWN: {
		vx = -BOOMERANG_BROTHER_SPEED;
		break;
	}
	case BOOMERANG_STATE_DESTROYED:
		break;
	}
}
