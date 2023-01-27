#include "MarioTail.h"
#include "Brick.h"

void CMarioTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == MARIO_TAIL_STATE_ATTACK) {
		l = x - MARIO_TAIL_BBOX_WIDTH / 2;
		t = y - MARIO_TAIL_BBOX_HEIGHT / 2;
		r = l + MARIO_TAIL_BBOX_WIDTH;
		b = t + MARIO_TAIL_BBOX_HEIGHT;
	}
}


void CMarioTail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

};

void CMarioTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
}

void CMarioTail::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = (CBrick*)e->obj;

	if (brick->GetState() == BRICK_STATE_IDLE) {
		brick->SetState(BRICK_STATE_BREAK);
	}
}

void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMarioTail::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_TAIL_STATE_ATTACK: {
		vx = nx * MARIO_TAIL_MOVING_SPEED;
		break;
	}
	default:
		vx = 0;
		break;
	}
}
