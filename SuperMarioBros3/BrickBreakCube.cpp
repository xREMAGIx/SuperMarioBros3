#include "BrickBreakCube.h"

void CBrickBreakCube::Render()
{
	int ani_id = ID_ANI_BRICK_BREAK_CUBE_LEFT;

	if (nx > 0) {
		ani_id = ID_ANI_BRICK_BREAK_CUBE_RIGHT;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ani_id)->Render(x, y);
	RenderBoundingBox();
}

void CBrickBreakCube::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BREAK_CUBE_BBOX_WIDTH / 2;
	t = y - BRICK_BREAK_CUBE_BBOX_HEIGHT / 2;
	r = l + BRICK_BREAK_CUBE_BBOX_WIDTH;
	b = t + BRICK_BREAK_CUBE_BBOX_HEIGHT;
}

void CBrickBreakCube::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_BREAK_CUBE_IDLE_STATE) {
		return;
	};

	if (state == BRICK_BREAK_CUBE_TIMEOUT_STATE) {
		isDeleted = true;
		return;
	};

	vy += ay * dt;

	x += vx * dt;
	y += vy * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}

void CBrickBreakCube::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICK_BREAK_CUBE_HIGH_FLY_STATE: {
		ay = BRICK_BREAK_CUBE_GRAVITY;
		vx = nx * BRICK_BREAK_CUBE_HIGH_FLY_SPEED_X;
		vy = -BRICK_BREAK_CUBE_HIGH_FLY_SPEED_Y;
		renderOrder = 2;
		break;
	}
	case BRICK_BREAK_CUBE_LOW_FLY_STATE: {
		ay = BRICK_BREAK_CUBE_GRAVITY;
		vx = nx * BRICK_BREAK_CUBE_LOW_FLY_SPEED_X;
		vy = -BRICK_BREAK_CUBE_LOW_FLY_SPEED_Y;
		renderOrder = 2;
		break;
	}
	default:
		break;
	}
}
