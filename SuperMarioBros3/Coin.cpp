#include "Coin.h"
#include "debug.h"
#include "PlayScene.h"

void CCoin::Render()
{
	if (state == COIN_STATE_JUMP_POINT) {
		point->Render();
	}
	else {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_COIN)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == COIN_STATE_JUMP) {
		vy += ay * dt;
		y += vy * dt;

		if ((GetTickCount64() - jump_start > COIN_JUMP_TIME))
		{
			SetState(COIN_STATE_JUMP_POINT);
			return;
		}
	}
	else if (state == COIN_STATE_JUMP_POINT) {
		point->Update(dt, coObjects);

		if (point->IsDeleted())
		{
			isDeleted = true;
			return;
		}
	}

	CGameObject::Update(dt, coObjects);
}


void CCoin::SetState(int state)
{
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);

	switch (state)
	{
	case COIN_STATE_EARNED: 
		if (dynamic_cast<CPlayScene*>(scene))
		{
			playScene->GetGameBoard()->AddPoint(COIN_POINT);
		}
		break;
	case COIN_STATE_JUMP: 
		jump_start = GetTickCount64();
		ay = COIN_GRAVITY;
		vy = -COIN_JUMP_SPEED;
		break;
	case COIN_STATE_JUMP_POINT: 
		point->SetPosition(this->x, this->y - 16);
		point->SetState(POINT_STATE_SHOW);
		if (dynamic_cast<CPlayScene*>(scene))
		{
			playScene->GetGameBoard()->AddPoint(COIN_POINT);
		}
		break;
	default:
		break;
	}

	CGameObject::SetState(state);
}
