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

		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		DebugOut(L"[INFO] Size: %d\n", coEventsResult.size());

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


			DebugOut(L"[INFO] Touch with tail id: %d\n", e->obj->GetId());

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is CGoomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				goomba->SetUpSideDown(1);
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
			}

			if (dynamic_cast<CVenusFireTrap*>(e->obj)) // if e->obj is CGoomba 
			{
				DebugOut(L"[INFO] Touch with tail id: %d\n", e->obj->GetId());

				CVenusFireTrap* venus = dynamic_cast<CVenusFireTrap*>(e->obj);
				venus->SetState(VENUS_FIRE_TRAP_STATE_DIE);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioTail::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20022);
	sprite->Draw(x + MARIO_TAIL_OFFSET_WIDTH, y + MARIO_TAIL_OFFSET_HEIGHT);
}

void CMarioTail::SetState(int state)
{
	CGameObject::SetState(state);
}
