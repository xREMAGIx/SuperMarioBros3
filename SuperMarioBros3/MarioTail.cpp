#include "Game.h"
#include "MarioTail.h"


CMarioTail::CMarioTail()
{
}

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx < 0) {
		left = x + MARIO_TAIL_OFFSET_WIDTH * 2 - MARIO_TAIL_BBOX_WIDTH;
		top = y + MARIO_TAIL_OFFSET_HEIGHT;
		right = x + MARIO_TAIL_OFFSET_WIDTH * 2;
		bottom = y + MARIO_TAIL_OFFSET_HEIGHT + MARIO_TAIL_BBOX_HEIGHT;
	}
	else {
		left = x + MARIO_TAIL_OFFSET_WIDTH;
		top = y + MARIO_TAIL_OFFSET_HEIGHT;
		right = x + MARIO_TAIL_OFFSET_WIDTH + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_OFFSET_HEIGHT + MARIO_TAIL_BBOX_HEIGHT;
	}
	
}

void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
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

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			DebugOut(L"[INFO] Touch with tail\n");

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Block 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioTail::Render()
{	
}

void CMarioTail::SetState(int state)
{
	CGameObject::SetState(state);
}
