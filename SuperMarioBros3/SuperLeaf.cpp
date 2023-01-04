#include "SuperLeaf.h"

CSuperLeaf::CSuperLeaf(float x, float y) :CGameObject(x, y)
{
	start_fly = -1;
	start_change_direction = -1;
	ay = SUPER_LEAF_GRAVITY;
	state = SUPER_LEAF_STATE_IDLE;
}

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SUPER_LEAF_BBOX_WIDTH / 2;
	top = y - SUPER_LEAF_BBOX_HEIGHT / 2;
	right = left + SUPER_LEAF_BBOX_WIDTH;
	bottom = top + SUPER_LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CSuperLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == SUPER_LEAF_STATE_EARNED) {
		isDeleted = true;
		return;
	}

	if (state == SUPER_LEAF_STATE_FLY_UP) {
		if (GetTickCount64() - start_fly > SUPER_LEFT_FLY_TIME)
		{
			SetState(SUPER_LEAF_STATE_FALL);
		}
	}

	if (state == SUPER_LEAF_STATE_FALL || state == SUPER_LEAF_STATE_CHANGE_DIRECTION) {

		if (GetTickCount64() - start_change_direction > SUPER_LEFT_CHANGE_DIRECTION_TIME / 3)
		{
			vx = nx * SUPER_LEAF_SPEED_X;
			vy = -SUPER_LEAF_BACK_Y;
		}

		if (GetTickCount64() - start_change_direction > SUPER_LEFT_CHANGE_DIRECTION_TIME)
		{
			SetState(SUPER_LEAF_STATE_CHANGE_DIRECTION);
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}


void CSuperLeaf::Render()
{
	CSprites* sprites = CSprites::GetInstance();

	int spriteId = ID_SPRITE_SUPER_LEAF_LEFT;

	if (nx > 0) {
		spriteId = ID_SPRITE_SUPER_LEAF_RIGHT;
	}

	sprites->Get(spriteId)->Draw(x, y);
	// RenderBoundingBox();
}

void CSuperLeaf::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case SUPER_LEAF_STATE_FLY_UP:{
			vx = 0.0f;
			vy = -SUPER_LEAF_FLY_UP_Y;
			StartFly();
			break;
		}
		case SUPER_LEAF_STATE_FALL: {
			vy = 0.0f;
			StartChangeDirection();
			break;
		}
		case SUPER_LEAF_STATE_CHANGE_DIRECTION: {
			nx = -nx;
			vx = nx * SUPER_LEAF_SPEED_X;
			StartChangeDirection();
			break;
		}
		default:
			break;
	}
}
