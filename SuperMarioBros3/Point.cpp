#include "Point.h"

void CPoint::Render()
{
	int spriteId = ID_SPRITE_POINT_100;
	CSprites* sprites = CSprites::GetInstance();
	switch (type)
	{
	case POINT_TYPE_100: 
	{	
		spriteId = ID_SPRITE_POINT_100;
		break;
	}
	case POINT_TYPE_200:
	{
		spriteId = ID_SPRITE_POINT_200;
		break;
	}
	case POINT_TYPE_400:
	{
		spriteId = ID_SPRITE_POINT_400;
		break;
	}
	case POINT_TYPE_800:
	{
		spriteId = ID_SPRITE_POINT_800;
		break;
	}
	case POINT_TYPE_1000:
	{
		spriteId = ID_SPRITE_POINT_1000;
		break;
	}
	case POINT_TYPE_2000:
	{
		spriteId = ID_SPRITE_POINT_2000;
		break;
	}
	case POINT_TYPE_4000:
	{
		spriteId = ID_SPRITE_POINT_4000;
		break;
	}
	case POINT_TYPE_8000:
	{
		spriteId = ID_SPRITE_POINT_8000;
		break;
	}
	case POINT_TYPE_UP:
	{
		spriteId = ID_SPRITE_POINT_UP;
		break;
	}
	default:
		break;
	}
	sprites->Get(spriteId)->Draw(x, y);
	//RenderBoundingBox();
}

void CPoint::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CPoint::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if ((state == POINT_STATE_SHOW) && (GetTickCount64() - show_start > POINT_SHOW_TIME))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPoint::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == POINT_STATE_SHOW) {
		vy = -POINT_FLOAT;
		show_start = GetTickCount64();
	}
}

void CPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}