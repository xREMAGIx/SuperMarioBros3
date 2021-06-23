#include "Game.h"
#include "GreenKoopa.h"

CGreenKoopa::CGreenKoopa()
{
	SetState(GREEN_KOOPA_STATE_WALKING);
}


void CGreenKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GREEN_KOOPA_BBOX_WIDTH;

	if (state == GREEN_KOOPA_STATE_DIE)
		bottom = y + GREEN_KOOPA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GREEN_KOOPA_BBOX_HEIGHT;
}

void CGreenKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += GREEN_KOOPA_GRAVITY * dt;

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
				if (e->nx != 0)
				{
					x += dx;
					y += dy;
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
			if (dynamic_cast<CGreenKoopa*>(e->obj)) // if e->obj is Block 
			{
				CGreenKoopa* block = dynamic_cast<CGreenKoopa*>(e->obj);
				if (e->nx != 0)
				{
					vx = -vx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGreenKoopa::Render()
{

	int ani = GREEN_KOOPA_ANI_WALKING;
	if (state == GREEN_KOOPA_STATE_DIE) {
		ani = GREEN_KOOPA_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CGreenKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_KOOPA_STATE_DIE:
		y += GREEN_KOOPA_BBOX_HEIGHT - GREEN_KOOPA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GREEN_KOOPA_STATE_WALKING:
		vx = -GREEN_KOOPA_SPEED;
	}
}
