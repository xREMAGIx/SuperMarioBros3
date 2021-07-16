#include "Game.h"
#include "Goomba.h"
#include "VenusFireTrap.h"

CVenusFireTrap::CVenusFireTrap()
{
	SetState(VENUS_FIRE_TRAP_STATE_SHOWING);
	//score = new CPoint();
	//score->SetPointId(POINT_ID_100);
}


void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x; 
	top = y;
	right = x + VENUS_FIRE_TRAP_BBOX_WIDTH;
	bottom = y + VENUS_FIRE_TRAP_BBOX_HEIGHT;
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != VENUS_FIRE_TRAP_STATE_HIDDEN)
	{
		CalcPotentialCollisions(coObjects, coEvents);
		//score->Update(dt, coObjects);
	}

	if (state == VENUS_FIRE_TRAP_STATE_HIDDEN) {
		//score->Update(dt, coObjects);
	}
	/*
	if (dt_show != 0 && GetTickCount() - dt_show > VENUS_FIRE_TRAP_TIME_SHOW)
	{
		DebugOut(L"[INFO] Load file map resources\n");
		SetState(VENUS_FIRE_TRAP_STATE_SHOWING);
	}
	*/

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
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CVenusFireTrap::Render()
{
	animation_set->at(0)->Render(x, y, -1, 255);
}

void CVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case VENUS_FIRE_TRAP_STATE_HIDDEN: {
			y -= VENUS_FIRE_TRAP_BBOX_HEIGHT;
			StartShow();
			break;
		} 
		case VENUS_FIRE_TRAP_STATE_SHOWING: {
			//y += VENUS_FIRE_TRAP_BBOX_HEIGHT;
			//StartShow();
			break;
		}
		default: 
			break;
	}
}
