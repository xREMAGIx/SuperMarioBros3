#include "Game.h"
#include "Coin.h"


CCoin::CCoin()
{
	SetState(COIN_STATE_IDLE);
	score = new CPoint();
	score->SetPointId(POINT_ID_100);
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == COIN_STATE_IDLE) {
		left = x;
		top = y;
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;
	}
	else if (state != COIN_STATE_EARNED) {
		left = x;
		top = y;
		right = x + 1;
		bottom = y + 1;
	}
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (state == COIN_STATE_JUMP && GetTickCount() - dt_jump > (TIME_COIN_JUMP / 2)) {
		vy += COIN_GRAVITY * dt;

		if (GetTickCount() - dt_jump > TIME_COIN_JUMP)
		{
			score->SetPosition(x, y - 18);
			score->SetState(POINT_STATE_SHOW);
			SetState(COIN_STATE_JUMP_POINT);
		}
	}
	if (state == COIN_STATE_JUMP_POINT) {
		score->Update(dt, coObjects);

		if (GetTickCount() - dt_point_jump > TIME_POINT_JUMP) {
			SetState(COIN_STATE_EARNED);
		}
	}


	x += dx;
	y += dy;
}

void CCoin::Render()
{
	int ani = COIN_ANI_IDLE;
	if (state == COIN_STATE_JUMP_POINT) {
		score->Render();
		ani = COIN_ANI_EARNED;
	}
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
		case COIN_STATE_JUMP: {
			StartJump();
			vy = -COIN_JUMP_SPEED;
			break;
		}
		case COIN_STATE_JUMP_POINT: {
			StartPointJump();
			break;
		}
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
