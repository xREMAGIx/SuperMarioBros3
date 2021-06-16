#include "Mushroom.h"

void CMushroom::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20001);
	sprite->Draw(x, y);
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSHSHROOM_BBOX_WIDTH;
	b = y + MUSHSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHSHROOM_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case MUSHSHROOM_STATE_WALKING:
		vx = -MUSHSHROOM_SPEED;
	}
}
