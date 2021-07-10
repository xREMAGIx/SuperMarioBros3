#include "Game.h"
#include "Coin.h"

CCoin::CCoin()
{
	SetState(COIN_STATE_IDLE);
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == COIN_STATE_IDLE) {
		left = x;
		top = y;
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;
	}
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CCoin::Render()
{

	int ani = COIN_ANI_IDLE;
	if (state == COIN_STATE_EARNED) {
		ani = COIN_ANI_EARNED;
	}
	if (ani != -1) {
		animation_set->at(ani)->Render(x, y, -1);
	}
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
}
