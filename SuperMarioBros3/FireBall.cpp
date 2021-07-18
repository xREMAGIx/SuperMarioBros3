#include "Game.h"
#include "FireBall.h"


CFireball::CFireball()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FIREBALL_ANI_SET_ID);
	SetAnimationSet(ani_set);
	SetState(FIREBALL_STATE_DESTROYED);
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_WIDTH;
	bottom = y + FIREBALL_HEIGHT;
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CMario*>(e->obj)) // if e->obj is Block 
			{
				LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
				CMario* mario = ((CPlayScene*)scene)->GetPlayer();
				if (mario->GetUntouchable() == 0)
				{

					if (mario->GetLevel() > MARIO_LEVEL_SMALL)
					{
						mario->SetLevel(MARIO_LEVEL_SMALL);
						mario->StartUntouchable();
					}
					else {
						DebugOut(L"[INFO] Touch Fireball Die\n");
						mario->SetState(MARIO_STATE_DIE);
					}
				}
			}
			else {
				x += dx;
				y += dy;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CFireball::Render()
{
	int ani = FIREBALL_ANI_THROWN;
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, nx, 255);
	}
}

void CFireball::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == FIREBALL_STATE_THROWN) {
		vx = nx*FIREBALL_SPEED;
		vy = ny*FIREBALL_SPEED;
	}
}
