#include "Game.h"
#include "ParaKoopa.h"

CParaKoopa::CParaKoopa()
{
	this->wing = new CBigWing(this->x + PARA_KOOPA_WING_X, this->y + PARA_KOOPA_WING_Y);
	SetState(PARA_KOOPA_STATE_WALKING);
}


void CParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PARA_KOOPA_BBOX_WIDTH;

	if (state == PARA_KOOPA_STATE_DIE)
		bottom = y + PARA_KOOPA_BBOX_HEIGHT_DIE;
	else
		bottom = y + PARA_KOOPA_BBOX_HEIGHT;
}

void CParaKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += PARA_KOOPA_GRAVITY * dt;

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
				x += dx;
			}

			//Touch question block
			if (dynamic_cast<CQuestionBlock*>(e->obj)) // if e->obj is Block 
			{
				CQuestionBlock* block = dynamic_cast<CQuestionBlock*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -vx;
					this->nx = -nx;
				}

			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				CEnemyWall* block = dynamic_cast<CEnemyWall*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -vx;
					this->nx = -nx;
				}
			}

			if (dynamic_cast<CParaKoopa*>(e->obj)) // if e->obj is Block 
			{
				CParaKoopa* block = dynamic_cast<CParaKoopa*>(e->obj);
				x += dx;
				y += dy;
			}

			if (dynamic_cast<CGreenKoopa*>(e->obj)) // if e->obj is Block 
			{
				CGreenKoopa* block = dynamic_cast<CGreenKoopa*>(e->obj);
				x += dx;
				y += dy;
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	wing->nx = nx;
	if (nx < 0) {
		wing->x = x + 0;
		wing->y = y + PARA_KOOPA_WING_Y;
	}
	else {
		wing->x = x + PARA_KOOPA_WING_X;
		wing->y = y + PARA_KOOPA_WING_Y;
	}
}

void CParaKoopa::Render()
{

	int ani = PARA_KOOPA_ANI_WALKING;
	if (state == PARA_KOOPA_STATE_DIE) {
		ani = PARA_KOOPA_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y, -nx, 255);
	wing->Render();
}

void CParaKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARA_KOOPA_STATE_DIE:
		y += PARA_KOOPA_BBOX_HEIGHT - PARA_KOOPA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case PARA_KOOPA_STATE_WALKING:
		vx = -PARA_KOOPA_SPEED;
	}
}
