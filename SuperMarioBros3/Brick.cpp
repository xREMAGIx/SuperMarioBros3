#include "Game.h"
#include "Brick.h"

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = y + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CBrick::Render()
{

	int ani = BRICK_ANI_IDLE;
	if (state == BRICK_STATE_BREAK) {
		ani = BRICK_ANI_BREAK;
	}
	animation_set->at(ani)->Render(x, y, -1, 255);
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
}
