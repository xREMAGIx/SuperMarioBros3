#include "Game.h"
#include "MarioFireball.h"


CMarioFireball::CMarioFireball()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FIREBALL_ANI_SET_ID);
	SetAnimationSet(ani_set);
	SetState(MARIO_FIREBALL_STATE_DESTROYED);
}

void CMarioFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == MARIO_FIREBALL_STATE_THROWN) {
		left = x;
		top = y;
		right = x + MARIO_FIREBALL_WIDTH;
		bottom = y + MARIO_FIREBALL_HEIGHT;
	}
}

void CMarioFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vy += MARIO_FIREBALL_GRAVITY * dt;

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CInvisiblePlatform*>(e->obj)) // if e->obj is Block 
			{
				if (e->ny < 0)
				{
					vy = -MARIO_FIREBALL_SPEED * 2;
				}
			}

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				goomba->SetUpSideDown(1);
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
				SetState(MARIO_FIREBALL_STATE_DESTROYED);
			}

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				SetState(MARIO_FIREBALL_STATE_DESTROYED);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioFireball::Render()
{
	int ani = MARIO_FIREBALL_ANI_THROWN;
	if (state == MARIO_FIREBALL_STATE_DESTROYED) {
		ani = -1;
	}
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, nx, 255);
	}
}

void CMarioFireball::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == MARIO_FIREBALL_STATE_THROWN) {
		vx = nx * MARIO_FIREBALL_SPEED;
		vy = -MARIO_FIREBALL_SPEED*2;
	}
}
