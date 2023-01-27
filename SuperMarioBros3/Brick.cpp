#include "Brick.h"

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
	case BRICK_STATE_TURNED: {
		break;
	}
	case BRICK_STATE_BREAK: {
		topLeftCube->Update(dt, coObjects);
		bottomLeftCube->Update(dt, coObjects);
		topRightCube->Update(dt, coObjects);
		bottomRightCube->Update(dt, coObjects);

		if (GetTickCount64() - break_start > BRICK_BREAK_TIME) {
			topLeftCube->SetState(BRICK_BREAK_CUBE_TIMEOUT_STATE);
			bottomLeftCube->SetState(BRICK_BREAK_CUBE_TIMEOUT_STATE);
			topRightCube->SetState(BRICK_BREAK_CUBE_TIMEOUT_STATE);
			bottomRightCube->SetState(BRICK_BREAK_CUBE_TIMEOUT_STATE);

			isDeleted = true;
		}
		break;
	}
	default:
		break;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	switch (state)
	{
	case BRICK_STATE_TURNED: {
		break;
	}
	case BRICK_STATE_BREAK: {
		topLeftCube->Render();
		bottomLeftCube->Render();
		topRightCube->Render();
		bottomRightCube->Render();
		break;
	}
	default:
		animations->Get(ID_ANI_BRICK)->Render(x, y);
		break;
	}
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICK_STATE_TURNED: {

		break;
	}
	case BRICK_STATE_BREAK: {
		StartBreak();

		topLeftCube->SetDirection(-1, 0);
		topLeftCube->SetState(BRICK_BREAK_CUBE_HIGH_FLY_STATE);

		bottomLeftCube->SetDirection(-1, 0);
		bottomLeftCube->SetState(BRICK_BREAK_CUBE_LOW_FLY_STATE);

		topRightCube->SetDirection(1, 0);
		topRightCube->SetState(BRICK_BREAK_CUBE_HIGH_FLY_STATE);

		bottomRightCube->SetDirection(1, 0);
		bottomRightCube->SetState(BRICK_BREAK_CUBE_LOW_FLY_STATE);
		break;
	}
	default:
		break;
	}
}
