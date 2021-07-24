#include "Game.h"
#include "ParaGoomba.h"
#include "InvisibleBlock.h"

CParaGoomba::CParaGoomba(float x, float y)
{
	this->leftWing = new CSmallWing(this->x + PARA_GOOMBA_LEFT_WING_X, this->y + PARA_GOOMBA_LEFT_WING_Y);
	this->rightWing = new CSmallWing(this->x + PARA_GOOMBA_RIGHT_WING_X, this->y + PARA_GOOMBA_RIGHT_WING_Y);
	rightWing->nx = -1;
	SetState(PARA_GOOMBA_STATE_WALKING);
}


void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PARA_GOOMBA_BBOX_WIDTH;

	if (state == PARA_GOOMBA_STATE_DIE) {
		bottom = y + PARA_GOOMBA_BBOX_HEIGHT_DIE;
	}
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

	float current_vy = vy;
	float current_vx = vx;


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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.5f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//Touch CGoomba
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				this->vy = current_vy;
				this->vx = current_vx;
				if (e->ny < 0)
				{
					y += dy;
				}
				if (nx != 0)
				{
					x += dx;
				}
			}

			if (dynamic_cast<CInvisiblePlatform*>(e->obj)) // if e->obj is Block 
			{
				CInvisiblePlatform* block = dynamic_cast<CInvisiblePlatform*>(e->obj);
				if (state != PARA_GOOMBA_STATE_WALKING_WITHOUT_WING && state != PARA_GOOMBA_STATE_DIE) {
					if (e->ny < 0)
					{
						if (jumpCount == 12) {
							SetState(PARA_GOOMBA_STATE_JUMP_BIG);
							jumpCount = 0;
							leftWing->SetState(SMALLWING_STATE_IDLE);
							rightWing->SetState(SMALLWING_STATE_IDLE);
						}
						else if (jumpCount == 4 || jumpCount == 8) {
							SetState(PARA_GOOMBA_STATE_JUMP_SMALL);
							jumpCount++;
						}
						else {
							jumpCount++;
						}

					}
				}
			}

			//Touch question block
			if (dynamic_cast<CQuestionBlock*>(e->obj)) // if e->obj is Block 
			{
				CQuestionBlock* block = dynamic_cast<CQuestionBlock*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -nx;
				}
			}

			//Touch other Goomba
			if (dynamic_cast<CParaGoomba*>(e->obj)) // if e->obj is Block 
			{
			}

			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				CEnemyWall* block = dynamic_cast<CEnemyWall*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -nx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	leftWing->x = x + PARA_GOOMBA_LEFT_WING_X;
	leftWing->y = y + PARA_GOOMBA_LEFT_WING_Y;

	rightWing->x = x + PARA_GOOMBA_RIGHT_WING_X;
	rightWing->y = y + PARA_GOOMBA_RIGHT_WING_Y;
}

void CParaGoomba::Render()
{
	int ani;

	switch (state)
	{
	case PARA_GOOMBA_STATE_JUMP_SMALL: 
	case PARA_GOOMBA_STATE_JUMP_BIG:
	case PARA_GOOMBA_STATE_WALKING: {
		ani = PARA_GOOMBA_ANI_WALKING;
		leftWing->Render();
		rightWing->Render();
		break;
	}
	case PARA_GOOMBA_STATE_DIE: {
		ani = PARA_GOOMBA_ANI_DIE;
		break;
	}
	default:
		ani = PARA_GOOMBA_ANI_WALKING;
		break;
	}

	animation_set->at(ani)->Render(x, y, -nx, 255);
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case PARA_GOOMBA_STATE_DIE:
			y += - PARA_GOOMBA_BBOX_HEIGHT + PARA_GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case PARA_GOOMBA_STATE_JUMP_SMALL:
			vy = -PARA_GOOMBA_JUMP_SPEED;
			leftWing->SetState(SMALLWING_STATE_FLYING);
			rightWing->SetState(SMALLWING_STATE_FLYING);
			break;
		case PARA_GOOMBA_STATE_JUMP_BIG:
			vy = -PARA_GOOMBA_JUMP_SPEED * 3;
			leftWing->SetState(SMALLWING_STATE_FLYING);
			rightWing->SetState(SMALLWING_STATE_FLYING);
			break;
		case PARA_GOOMBA_STATE_WALKING_WITHOUT_WING:
		case PARA_GOOMBA_STATE_WALKING: {
			vx = -PARA_GOOMBA_SPEED;
		}
	}

}
