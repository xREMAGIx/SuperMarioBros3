#include "Game.h"
#include "Goomba.h"
#include "InvisibleBlock.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}


void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != GOOMBA_STATE_DIE)
	{
		vy += GOOMBA_GRAVITY * dt;
		CalcPotentialCollisions(coObjects, coEvents);
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (e->ny < 0)
				{
					 vy = 0;
				}
			}

			//Touch question block
			if (dynamic_cast<CQuestionBlock*>(e->obj)) // if e->obj is Block 
			{
				CQuestionBlock* block = dynamic_cast<CQuestionBlock*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
					nx = -nx;
				}
			}

			//Touch other Goomba
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* block = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
					nx = -nx;
				}
			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				CEnemyWall* block = dynamic_cast<CEnemyWall*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
					nx = -nx;
				}
			}

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				CChimney* block = dynamic_cast<CChimney*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
					nx = -nx;
				}
			}

			if (dynamic_cast<CInvisibleWall*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleWall* block = dynamic_cast<CInvisibleWall*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
					nx = -nx;
				}
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{

	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_SPEED;
	}
}
