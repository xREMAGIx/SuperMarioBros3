#include "FallDetector.h"
#include "debug.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

void CFallDetector::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CFallDetector::Render()
{
	RenderBoundingBox();
}

void CFallDetector::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth;
	b = t + this->cellHeight;
}

void CFallDetector::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFallDetector::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) {
		if (e->ny < 0) {
			SetState(FALL_DETECTOR_STATE_IDLE);
		}
	};
}

void CFallDetector::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == FALL_DETECTOR_STATE_FALL && (GetTickCount64() - fall_start > FALL_DETECTOR_FALL_TIMEOUT)) {
		SetState(FALL_DETECTOR_STATE_DETECT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFallDetector::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FALL_DETECTOR_STATE_FALL: 
		StartFall();
		vy = 0;
		break;
	default:
		break;
	}
}
