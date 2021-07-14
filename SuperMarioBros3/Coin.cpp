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
	CGameObject::Update(dt);
	
	if (dt_jump	!=0 && (GetTickCount() - dt_jump > TIME_COIN_JUMP) && state != COIN_STATE_EARNED)
	{
		SetState(COIN_STATE_EARNED);
	}
	

	x += dx;
	y += dy;
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

	switch (state)
	{
		case COIN_STATE_JUMP:
			StartJump();
			vy = -COIN_JUMP_SPEED;
			break;
		case COIN_STATE_EARNED: {
			CBoard* game_board = CBoard::GetInstance();
			game_board->AddPoint(100);
			game_board->AddMoney();
			break;
		}
		case COIN_STATE_IDLE:
			break;
	}
}
