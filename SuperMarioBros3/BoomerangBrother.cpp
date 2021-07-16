#include "Game.h"
#include "Goomba.h"
#include "BoomerangBrother.h"

CBoomerangBrother::CBoomerangBrother()
{

}


void CBoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BROTHER_BBOX_WIDTH;
	bottom = y + BOOMERANG_BROTHER_BBOX_HEIGHT;
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BOOMERANG_BROTHER_STATE_DIE)
	{
		vy += BOOMERANG_BROTHER_GRAVITY * dt;
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

			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBoomerangBrother::Render()
{
	int ani = BOOMERANG_BROTHER_ANI_WALKING;
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, -nx, 255);
	}
}

void CBoomerangBrother::SetState(int state)
{
}
