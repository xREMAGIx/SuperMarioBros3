#include "Game.h"
#include "ParaGoomba.h"
#include "InvisibleBlock.h"

CParaGoomba::CParaGoomba()
{
	SetState(PARA_GOOMBA_STATE_WALKING);
}


void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PARA_GOOMBA_BBOX_WIDTH;

	if (state == PARA_GOOMBA_STATE_DIE)
		bottom = y + PARA_GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + PARA_GOOMBA_BBOX_HEIGHT;
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += PARA_GOOMBA_GRAVITY * dt;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.5f;		// nx*0.5f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.5f;

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
				}
			}

			//Touch other Goomba
			if (dynamic_cast<CParaGoomba*>(e->obj)) // if e->obj is Block 
			{
				CParaGoomba* block = dynamic_cast<CParaGoomba*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
				}
			}

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* block = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					nx = -nx;
					vx = -vx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CParaGoomba::Render()
{

	int ani = PARA_GOOMBA_ANI_WALKING;
	if (state == PARA_GOOMBA_STATE_DIE) {
		ani = PARA_GOOMBA_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARA_GOOMBA_STATE_DIE:
		y += PARA_GOOMBA_BBOX_HEIGHT - PARA_GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case PARA_GOOMBA_STATE_WALKING:
		vx = -PARA_GOOMBA_SPEED;
	}
}
