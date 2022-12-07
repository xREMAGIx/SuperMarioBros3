#include "RedKoopa.h"

CRedKoopa::CRedKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = RED_KOOPA_GRAVITY;
	die_start = -1;
	SetState(RED_KOOPA_STATE_WALKING);
}

void CRedKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == RED_KOOPA_STATE_DIE)
	{
		left = x - RED_KOOPA_BBOX_WIDTH / 2;
		top = y - RED_KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + RED_KOOPA_BBOX_WIDTH;
		bottom = top + RED_KOOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - RED_KOOPA_BBOX_WIDTH / 2;
		top = y - RED_KOOPA_BBOX_HEIGHT / 2;
		right = left + RED_KOOPA_BBOX_WIDTH;
		bottom = top + RED_KOOPA_BBOX_HEIGHT;
	}
}

void CRedKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRedKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == RED_KOOPA_STATE_DIE) && (GetTickCount64() - die_start > RED_KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CRedKoopa::Render()
{
	int aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
	if (state == RED_KOOPA_STATE_DIE)
	{
		aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CRedKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RED_KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (RED_KOOPA_BBOX_HEIGHT - RED_KOOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case RED_KOOPA_STATE_WALKING:
		vx = -RED_KOOPA_WALKING_SPEED;
		break;
	}
}
 