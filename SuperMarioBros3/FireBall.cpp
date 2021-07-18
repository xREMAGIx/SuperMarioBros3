#include "Game.h"
#include "FireBall.h"


CFireball::CFireball()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FIREBALL_ANI_SET_ID);
	SetAnimationSet(ani_set);
	SetState(FIREBALL_STATE_DESTROYED);
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
	int ani = FIREBALL_ANI_THROWN;
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, nx, 255);
	}
}

void CFireball::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == FIREBALL_STATE_THROWN) {
		vx = nx*FIREBALL_SPEED;
		vy = ny*FIREBALL_SPEED;
	}
}
