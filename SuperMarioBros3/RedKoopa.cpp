#include "Game.h"
#include "RedKoopa.h"

CRedKoopa::CRedKoopa()
{
	SetState(RED_KOOPA_STATE_WALKING);
}


void CRedKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RED_KOOPA_BBOX_WIDTH;

	switch (state)
	{
		case RED_KOOPA_STATE_SHELL:
		case RED_KOOPA_STATE_SHELL_SCROLL: {
			bottom = y + RED_KOOPA_BBOX_SHELL_HEIGHT;
			break;
		}
		default:
			bottom = y + RED_KOOPA_BBOX_HEIGHT;
			break;
	}
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += RED_KOOPA_GRAVITY * dt;

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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->nx != 0)
			{
				this->vx = -vx;
				this->nx = -nx;
			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				if (state == RED_KOOPA_STATE_SHELL_SCROLL) {
					x += dx;
					y += dy;
				}
			}
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (e->nx != 0)
				{
					x += dx;
					y += dy;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CRedKoopa::Render()
{
	int ani ;
	
	switch (state)
	{
		case RED_KOOPA_STATE_RESPAWN:
		{
			ani = RED_KOOPA_ANI_RESPAWN;
			break;
		}
		case RED_KOOPA_STATE_SHELL:
		{
			ani = RED_KOOPA_ANI_SHELL;
			break;
		}
		case RED_KOOPA_STATE_SHELL_SCROLL:
		{
			ani = RED_KOOPA_ANI_SHELL_SCROLL;
			break;
		}
		default:
			ani = RED_KOOPA_ANI_WALKING;
			break;
	}

	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CRedKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case RED_KOOPA_STATE_RESPAWN: {
			y += - RED_KOOPA_BBOX_HEIGHT + RED_KOOPA_BBOX_SHELL_HEIGHT + 1;
			vx = 0;
			break;
		}
		case RED_KOOPA_STATE_SHELL: {
			y += - RED_KOOPA_BBOX_HEIGHT + RED_KOOPA_BBOX_SHELL_HEIGHT + 1;
			vx = 0;
			break;
		}
		case RED_KOOPA_STATE_SHELL_SCROLL: {
			vx = nx* RED_KOOPA_SCROLL;
			break;
		}
		case RED_KOOPA_STATE_WALKING: {
			vx = -RED_KOOPA_SPEED;
			break;
		}
	}
}
