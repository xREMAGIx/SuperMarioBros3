#include "Mushroom.h"

CMushroom::CMushroom(float x, float y) :CGameObject(x	, y)
{
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	this->vx = -MUSHROOM_WALKING_SPEED;
	this->point_show_start = -1;
	point = new CPoint(x, y - 16);
	point->SetType(POINT_TYPE_1000);
}

void CMushroom::Render()
{
	if (state != MUSHSHROOM_STATE_EARNED) {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
	}
	else {
		point->Render();
	}

	//RenderBoundingBox();
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == MUSHSHROOM_STATE_EARNED) {
		point->Update(dt, coObjects);

		if ((GetTickCount64() - point_show_start > POINT_SHOW_TIME))
		{
			point->Delete();
			isDeleted = true;
			return;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::SetState(int state)
{
	if (state == MUSHSHROOM_STATE_EARNED) {
		point_show_start = GetTickCount64();
		point->SetPosition(x, y - 16);
		point->SetState(POINT_STATE_SHOW);
	}

	CGameObject::SetState(state);
}
