#include "Fireball.h"

CFireball::CFireball(float x, float y):CGameObject(x, y)
{
	state = FIREBALL_STATE_IDLE;
}

void CFireball::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x - FIREBALL_BBOX_WIDTH / 2;
	top = y - FIREBALL_BBOX_HEIGHT / 2;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

void CFireball::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CMario*>(e->obj)) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
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
		SetState(FIREBALL_STATE_DESTROYED);
	}
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state = FIREBALL_STATE_DESTROYED)
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFireball::Render()
{
	if (state == FIREBALL_STATE_THROWN)
	{
		int aniId = ID_ANI_FIREBALL_THROWN;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
		RenderBoundingBox();
	}	
}

void CFireball::SetState(int state)
{
	switch (state)
	{
		if (state == FIREBALL_STATE_THROWN) {
			vx = nx * FIREBALL_SPEED;
			vy = ny * FIREBALL_SPEED;
		}
	}
	CGameObject::SetState(state);
}
