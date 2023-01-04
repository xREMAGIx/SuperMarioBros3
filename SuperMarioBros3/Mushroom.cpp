#include "Mushroom.h"
#include "PlayScene.h"

CMushroom::CMushroom(float x, float y) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	dt_start_show = -1;
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

	// RenderBoundingBox();
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

	if (state == MUSHSHROOM_STATE_EARNED) {
		point->Update(dt, coObjects);

		if (point->IsDeleted())
		{
			isDeleted = true;
			return;
		}
	}

	CGameObject::Update(dt, coObjects);

	if (state == MUSHSHROOM_STATE_SHOWING) {

		if (GetTickCount64() - dt_start_show > MUSHSHROOM_SHOW_TIME) {
			SetState(MUSHSHROOM_STATE_RUNNING);
			return;
		}

		CCollision::GetInstance()->Process(this, dt, coObjects, true);
	}

	if (state == MUSHSHROOM_STATE_RUNNING) {
		vy += ay * dt;
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CMushroom::SetState(int state)
{
	switch (state)
	{
		case MUSHSHROOM_STATE_SHOWING: {
			StartShow();
			vy = -MUSHROOM_SHOW_SPEED;
			break;
		}
		case MUSHSHROOM_STATE_RUNNING: {
			ay = MUSHROOM_GRAVITY;
			vy = 0;
			vx = -MUSHROOM_WALKING_SPEED;
			break;
		}
		case MUSHSHROOM_STATE_EARNED: {
			point->SetPosition(x, y - 16);
			point->SetState(POINT_STATE_SHOW);

			LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
			if (dynamic_cast<CPlayScene*>(scene))
			{
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				playScene->GetGameBoard()->AddPoint(MUSHSHROOM_POINT);
			}
			break;
		}
		default:
			break;
	}

	CGameObject::SetState(state);
}
