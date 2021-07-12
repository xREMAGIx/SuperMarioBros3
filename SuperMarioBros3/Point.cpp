#include "Game.h"
#include "Point.h"

CPoint::CPoint()
{
	SetState(POINT_STATE_HIDE);
}

void CPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (dt_jump != 0 && (GetTickCount() - dt_jump > TIME_POINT_JUMP))
	{
		SetState(POINT_STATE_HIDE);
	}

	x += dx;
	y += dy;
}

void CPoint::Render()
{
	if (state == POINT_STATE_SHOW) {
		LPSPRITE sprite;
		switch (point_id)
		{
			case POINT_ID_100:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_100);
				break;
			case POINT_ID_200:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_200);
				break;
			case POINT_ID_400:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_400);
				break;
			case POINT_ID_800:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_800);
				break;
			case POINT_ID_1000:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_1000);
				break;
			case POINT_ID_2000:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_2000);
				break;
			case POINT_ID_4000:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_4000);
				break;
			case POINT_ID_8000:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_8000);
				break;
			default:
				sprite = CSprites::GetInstance()->Get(POINT_SPRITE_ID_LEVELUP);
				break;
		}
		sprite->Draw(x, y);
	}
}

void CPoint::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case POINT_STATE_SHOW:
			StartJump();
			vy = -POINT_JUMP_SPEED;
			break;
		case POINT_STATE_HIDE:
			break;
	}
}
