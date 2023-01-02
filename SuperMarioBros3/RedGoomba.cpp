#include "RedGoomba.h"

CRedGoomba::CRedGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = RED_GOOMBA_GRAVITY;
	die_start = -1;
	SetState(RED_GOOMBA_STATE_WALKING);

	fallDetector = new CFallDetector(x, y, 8, 8);
}

void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == RED_GOOMBA_STATE_DIE)
	{
		left = x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT;
	}
}

void CRedGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRedGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		nx = -nx;
		vx = -vx;
	}
}

void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	bool skipBlockCollide = false;

	if ((state == RED_GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > RED_GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (state == RED_GOOMBA_STATE_JUMP_DIE)
	{
		skipBlockCollide = true;
		if (GetTickCount64() - die_start > GOOMBA_JUMP_DIE_TIMEOUT) {
			isDeleted = true;
			return;
		}
	}

	if (state == RED_GOOMBA_STATE_WALKING) {

		int fallDetectorState = fallDetector->GetState();

		if (fallDetectorState != FALL_DETECTOR_STATE_FALL)
		{
			if (fallDetectorState == FALL_DETECTOR_STATE_DETECT) {
				vx = -vx;
				nx = -nx;
			}

			float fallDetectorX, fallDetectorY;
			fallDetectorX = x + nx * (RED_GOOMBA_BBOX_WIDTH + 8);
			fallDetectorY = y - RED_GOOMBA_BBOX_HEIGHT / 2;
			fallDetector->SetPosition(fallDetectorX, fallDetectorY);
			fallDetector->SetState(FALL_DETECTOR_STATE_FALL);
		}

		fallDetector->Update(dt, coObjects);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, skipBlockCollide);
}


void CRedGoomba::Render()
{
	int aniId = ID_ANI_RED_GOOMBA_WALKING;
	bool flip = false;

	if (state == RED_GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_RED_GOOMBA_DIE;
	}

	if (state == RED_GOOMBA_STATE_JUMP_DIE) {
		flip = true;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y, flip);
	RenderBoundingBox();
}

void CRedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case RED_GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (RED_GOOMBA_BBOX_HEIGHT - RED_GOOMBA_BBOX_HEIGHT_DIE) / 2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case RED_GOOMBA_STATE_JUMP_DIE:
			vx = 0;
			vy = -RED_GOOMBA_JUMP_DIE_SPEED;
			die_start = GetTickCount64();
			break;
		case RED_GOOMBA_STATE_WALKING:
			nx = -1;
			vx = -RED_GOOMBA_WALKING_SPEED;
			break;
		default:
			break;
	}
}
