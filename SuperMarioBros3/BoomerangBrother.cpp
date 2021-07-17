#include "Game.h"
#include "Goomba.h"
#include "BoomerangBrother.h"

CBoomerangBrother::CBoomerangBrother()
{
	SetState(BOOMERANG_BROTHER_STATE_WALKING);
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

			if ((dt_throw != 0 && GetTickCount() - dt_throw > TIME_BOOMERANG_BROTHER_THROW))
			{
				DebugOut(L"[ERROR] Throw Boomerang!\n");
				SetState(BOOMERANG_BROTHER_STATE_THROW);
				StartWaiting();
				boomerang->SetPosition(x, y);
				boomerang->SetState(BOOMERANG_STATE_THROWN);
			}
			if (boomerang->GetState() == BOOMERANG_STATE_THROWN) {
				boomerang->Update(dt, coObjects);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBoomerangBrother::Render()
{
	int ani = BOOMERANG_BROTHER_ANI_WALKING;
	boomerang->Render();

	if (state == BOOMERANG_BROTHER_STATE_THROW) {
		boomerang->Render();
	}
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, -nx, 255);
	}
}

void CBoomerangBrother::SetState(int state)
{
	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_DIE: {
		score->SetPosition(x, y - 18);
		score->SetState(POINT_STATE_SHOW);
		CBoard* game_board = CBoard::GetInstance();
		game_board->AddPoint(100);
		break;
	}
	case BOOMERANG_BROTHER_STATE_THROW: {
		break;
	}
	case BOOMERANG_BROTHER_STATE_WALKING:
		boomerang = new CBoomerang();
		StartThrow();
		// vx = -BOOMERANG_BROTHER_SPEED;
		break;
	}
}
