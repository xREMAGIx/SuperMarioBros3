#include "Game.h"
#include "MarioFireball.h"


CMarioFireball::CMarioFireball()
{
	dt_destroy = 0;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(MARIO_FIREBALL_ANI_SET_ID);
	SetAnimationSet(ani_set);
	SetState(MARIO_FIREBALL_STATE_DESTROYED);
}

void CMarioFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != MARIO_FIREBALL_STATE_DESTROYED) {
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

	float current_vy = vy;
	float current_vx = vx;

	vy += MARIO_FIREBALL_GRAVITY * dt;

	CalcPotentialCollisions(coObjects, coEvents);

	if (dt_destroy != 0 && GetTickCount() - dt_destroy > MARIO_FIREBALL_DESTROYED_TIME)
	{
		SetState(MARIO_FIREBALL_STATE_DESTROYED);
		StopSmoke();
	}

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

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				goomba->SetUpSideDown(1);
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
				SetState(MARIO_FIREBALL_STATE_DESTROY);
			}

			if (dynamic_cast<CVenusFireTrap*>(e->obj)) // if e->obj is Block 
			{
				CVenusFireTrap* venus = dynamic_cast<CVenusFireTrap*>(e->obj);
				venus->SetState(VENUS_FIRE_TRAP_STATE_DIE);
				SetState(MARIO_FIREBALL_STATE_DESTROY);
			}

			if (dynamic_cast<CParaGoomba*>(e->obj)) // if e->obj is Block 
			{
				CParaGoomba* paragoomba = dynamic_cast<CParaGoomba*>(e->obj);
				paragoomba->SetUpSideDown(1);
				paragoomba->SetState(PARA_GOOMBA_STATE_JUMP_DIE);
				SetState(MARIO_FIREBALL_STATE_DESTROY);
			}

			if (dynamic_cast<CRedKoopa*>(e->obj)) // if e->obj is Block 
			{
				CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(e->obj);
				if (koopa->GetState() == RED_KOOPA_STATE_WALKING) {
					koopa->SetUpSideDown(1);
					koopa->SetState(RED_KOOPA_STATE_SHELL);
				}
				SetState(MARIO_FIREBALL_STATE_DESTROY);
			}

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				SetState(MARIO_FIREBALL_STATE_DESTROY);
			}

			if (dynamic_cast<CInvisiblePlatform*>(e->obj)) // if e->obj is Block 
			{
				if (e->ny < 0)
				{
					vy = -MARIO_FIREBALL_SPEED * 2;
				}

				if (e->nx != 0)
				{
					SetState(MARIO_FIREBALL_STATE_DESTROY);
				}
			}

			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				if (e->nx != 0) {
					this->vx = current_vx;
					x += dx;
				}

				if (e->ny > 0)
				{
					this->vy = current_vy;
					y += dy;
				}
				else {
					vy = -MARIO_FIREBALL_SPEED * 2;
				}
			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				if (e->nx != 0) {
					this->vx = current_vx;
					x += dx;
				}

				if (e->ny != 0)
				{
					this->vy = current_vy;
					y += dy;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioFireball::Render()
{
	int ani = MARIO_FIREBALL_ANI_THROWN;
	if (state == MARIO_FIREBALL_STATE_DESTROY) {
		ani = MARIO_FIREBALL_ANI_DESTROYED;
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
	else if (state == MARIO_FIREBALL_STATE_DESTROY) {
		StartSmoke();
	}
}
