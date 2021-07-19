#include "Game.h"
#include "ParaKoopa.h"

CParaKoopa::CParaKoopa()
{
	this->wing = new CBigWing(this->x + PARA_KOOPA_WING_X, this->y + PARA_KOOPA_WING_Y);
	SetState(PARA_KOOPA_STATE_JUMP);
}


void CParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PARA_KOOPA_BBOX_WIDTH;

	switch (state)
	{
	case PARA_KOOPA_STATE_SHELL:
	case PARA_KOOPA_STATE_SHELL_SCROLL:
	case PARA_KOOPA_STATE_SHELL_HOLD: {
		bottom = y + PARA_KOOPA_BBOX_SHELL_HEIGHT;
		break;
	}
	default:
		bottom = y + PARA_KOOPA_BBOX_HEIGHT;
		break;
	}
}

void CParaKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += PARA_KOOPA_GRAVITY * dt;

	float current_vy = vy;
	float current_vx = vx;

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
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.5f;		// nx*0.5f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.5f; 

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (state == PARA_KOOPA_STATE_FLY) {
					if (e->ny < 0)
					{
						SetState(PARA_KOOPA_STATE_JUMP);
					}
				}
				x += dx;
			}

			if (dynamic_cast<CInvisiblePlatform*>(e->obj)) // if e->obj is Block 
			{
				CInvisiblePlatform* block = dynamic_cast<CInvisiblePlatform*>(e->obj);
				if (state == PARA_KOOPA_STATE_FLY) {
					if (e->ny < 0)
					{
						SetState(PARA_KOOPA_STATE_JUMP);
					}
				}
			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				this->vy = current_vy;
				this->vx = current_vx;
				x += dx;
				y += dy;
			}

			/*
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
				if (nx != 0) vx = 0;
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
			*/

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
	int ani;
	switch (state)
	{
	case PARA_KOOPA_STATE_FLY:
	case PARA_KOOPA_STATE_JUMP: {
		ani = PARA_KOOPA_ANI_WALKING;
		wing->Render();
		break;
	}
	case PARA_KOOPA_STATE_RESPAWN:
	{
		ani = PARA_KOOPA_ANI_RESPAWN;
		break;
	}
	case PARA_KOOPA_STATE_SHELL:
	case PARA_KOOPA_STATE_SHELL_HOLD:
	{
		ani = PARA_KOOPA_ANI_SHELL;
		break;
	}
	case PARA_KOOPA_STATE_SHELL_SCROLL:
	{
		ani = PARA_KOOPA_ANI_SHELL_SCROLL;
		break;
	}
	default:
		ani = PARA_KOOPA_ANI_WALKING;
		break;
	}

	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CParaKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case PARA_KOOPA_STATE_JUMP: {
			vy = -PARA_KOOPA_JUMP_SPEED;
			vx = -PARA_KOOPA_SPEED;
			SetState(PARA_KOOPA_STATE_FLY);
			break;
		}
		case PARA_KOOPA_STATE_RESPAWN: {
			y += -PARA_KOOPA_BBOX_HEIGHT + PARA_KOOPA_BBOX_SHELL_HEIGHT + 1;
			vx = 0;
			break;
		}
		case PARA_KOOPA_STATE_SHELL: {
			y += -PARA_KOOPA_BBOX_HEIGHT + PARA_KOOPA_BBOX_SHELL_HEIGHT + 1;
			vx = 0;
			break;
		}
		case PARA_KOOPA_STATE_SHELL_SCROLL: {
			vx = nx * PARA_KOOPA_SCROLL;
			break;
		}
		case PARA_KOOPA_STATE_WALKING: {
			vx = -PARA_KOOPA_SPEED;
			break;
		}
	}
}
