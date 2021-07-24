#include "Game.h"
#include "Goomba.h"
#include "InvisibleBlock.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
	score = new CPoint();
	score->SetPointId(POINT_ID_100);
}


void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	bottom = y + GOOMBA_BBOX_HEIGHT;
	
	if (state == GOOMBA_STATE_JUMP_DIE) {
		right = x + 1;
		bottom = y + 1;

		if ((dt_die != 0 && GetTickCount() - dt_die > TIME_GOOMBA_DIE))
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}
	}

	if (state == GOOMBA_STATE_DIE) {
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;

		if ((dt_die != 0 &&  GetTickCount() - dt_die > TIME_GOOMBA_DIE ))
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}
	}
	
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GOOMBA_STATE_DIE) {
		vy += GOOMBA_GRAVITY * dt;	
		CalcPotentialCollisions(coObjects, coEvents);
	}

	float current_vy = vy;
	float current_vx = vx;
	
	if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_JUMP_DIE) {
		score->Update(dt, coObjects);
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (e->nx != 0) {
					this->vx = current_vx;
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
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* block = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -nx;
				}
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

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				CChimney* block = dynamic_cast<CChimney*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -nx;
				}
			}

			if (dynamic_cast<CInvisiblePlatform*>(e->obj)) // if e->obj is Block 
			{
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -nx;
				}
			}

			if (dynamic_cast<CRedKoopa*>(e->obj)) // if e->obj is Block 
			{
				CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(e->obj);
				if (e->nx != 0)
				{
					if (koopa->GetState() == RED_KOOPA_STATE_SHELL_SCROLL) {
						SetState(GOOMBA_STATE_JUMP_DIE);
					}
				}
			}

			if (dynamic_cast<CInvisibleWall*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleWall* invisibleWall = dynamic_cast<CInvisibleWall*>(e->obj);
				if (e->nx != 0)
				{
					this->vx = -current_vx;
					this->nx = -nx;
				}
			}

			if (dynamic_cast<CMario*>(e->obj)) // if e->obj is Mario 
			{	
				if (nx != 0) vx = 0;
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
		score->Render();
	}
	if (state == GOOMBA_STATE_JUMP_DIE) {
		score->Render();
	}
	if (dt_die != 0 && GetTickCount() - dt_die > TIME_GOOMBA_DIE)
	{
		ani = -1;
	}
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, -nx, 255);
	}
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE: {
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			score->SetPosition(x, y - 18);
			score->SetState(POINT_STATE_SHOW);
			vx = 0;
			vy = 0;
			CBoard* game_board = CBoard::GetInstance();
			game_board->AddPoint(100);
			break;
		}
		case GOOMBA_STATE_JUMP_DIE: {
			vy = -GOOMBA_JUMP_SPEED;
			vx = 0;
			CBoard* game_board = CBoard::GetInstance();
			game_board->AddPoint(100);
			StartDie();
			break;
		}
		case GOOMBA_STATE_WALKING: {
			vx = -GOOMBA_SPEED;
			break;
		}
	}
}
