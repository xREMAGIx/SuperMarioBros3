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

	switch (state)
	{
	case GREEN_KOOPA_STATE_SHELL:
	case GREEN_KOOPA_STATE_SHELL_SCROLL:
	case GREEN_KOOPA_STATE_SHELL_HOLD: {
		bottom = y + GREEN_KOOPA_BBOX_SHELL_HEIGHT;
		break;
	}
	default:
		bottom = y + GREEN_KOOPA_BBOX_HEIGHT;
		break;
	}
}

void CGreenKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += GREEN_KOOPA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	float current_vy = vy;
	float current_vx = vx;

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
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Walk on invisible wall
			if (dynamic_cast<CInvisibleWall*>(e->obj)) // if e->obj is Block 
			{
				if (e->nx != 0) {
					this->vx = -current_vx;
					this->nx = -this->nx;
				}
			}

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
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
					if (state == RED_KOOPA_STATE_SHELL_SCROLL) {
						block->SetState(QUESTION_BLOCK_STATE_OPENED);
					}
					this->vx = -current_vx;
					this->nx = -this->nx;
				}

			}

			//Touch other Goomba
			if (dynamic_cast<CRedKoopa*>(e->obj)) // if e->obj is Block 
			{
				CRedKoopa* block = dynamic_cast<CRedKoopa*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -this->nx;
				}
			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				if (state != RED_KOOPA_STATE_SHELL_SCROLL) {
					CEnemyWall* block = dynamic_cast<CEnemyWall*>(e->obj);
					if (e->nx != 0)
					{
						this->vx = -current_vx;
						this->nx = -this->nx;
					}
				}
				else {
					this->vy = current_vy;
					this->vx = current_vx;
					x += dx;
					y += dy;
				}
			}

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				CChimney* block = dynamic_cast<CChimney*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -this->nx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGreenKoopa::Render()
{
	int ani;

	switch (state)
	{
	case GREEN_KOOPA_STATE_RESPAWN:
	{
		ani = GREEN_KOOPA_ANI_RESPAWN;
		break;
	}
	case GREEN_KOOPA_STATE_SHELL:
	case GREEN_KOOPA_STATE_SHELL_HOLD:
	{
		ani = GREEN_KOOPA_ANI_SHELL;
		break;
	}
	case GREEN_KOOPA_STATE_SHELL_SCROLL:
	{
		ani = GREEN_KOOPA_ANI_SHELL_SCROLL;
		break;
	}
	default:
		ani = GREEN_KOOPA_ANI_WALKING;
		break;
	}


	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CGreenKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_KOOPA_STATE_RESPAWN: {
		y += -GREEN_KOOPA_BBOX_HEIGHT + GREEN_KOOPA_BBOX_SHELL_HEIGHT + 1;
		vx = 0;
		break;
	}
	case GREEN_KOOPA_STATE_SHELL: {
		y += -GREEN_KOOPA_BBOX_HEIGHT + GREEN_KOOPA_BBOX_SHELL_HEIGHT + 1;
		vx = 0;
		break;
	}
	case GREEN_KOOPA_STATE_SHELL_SCROLL: {
		vx = nx * GREEN_KOOPA_SCROLL;
		break;
	}
	case GREEN_KOOPA_STATE_WALKING: {
		vx = -GREEN_KOOPA_SPEED;
		break;
	}
	}
}
