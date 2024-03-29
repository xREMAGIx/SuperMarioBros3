#include "GreenMushroom.h"
#include "PlayScene.h"

CGreenMushroom::CGreenMushroom(float x, float y) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	dt_start_show = -1;
	point = new CPoint(x, y - GREEN_MUSHSHROOM_POINT_OFFSET_Y);
	point->SetType(POINT_TYPE_UP);
}

void CGreenMushroom::Render()
{
	if (state != GREEN_MUSHSHROOM_STATE_EARNED) {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_GREEN_MUSHROOM)->Render(x, y);
	}
	else {
		point->Render();
	}

	//RenderBoundingBox();
}

void CGreenMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GREEN_MUSHROOM_BBOX_WIDTH / 2;
	t = y - GREEN_MUSHROOM_BBOX_HEIGHT / 2;
	r = l + GREEN_MUSHROOM_BBOX_WIDTH;
	b = t + GREEN_MUSHROOM_BBOX_HEIGHT;
}

void CGreenMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGreenMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
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

void CGreenMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == GREEN_MUSHSHROOM_STATE_EARNED) {
		point->Update(dt, coObjects);

		if (point->IsDeleted())
		{
			isDeleted = true;
			return;
		}
	}

	CGameObject::Update(dt, coObjects);

	if (state == GREEN_MUSHSHROOM_STATE_SHOWING) {

		if (GetTickCount64() - dt_start_show > GREEN_MUSHSHROOM_SHOW_TIME) {
			SetState(GREEN_MUSHSHROOM_STATE_RUNNING);
			return;
		}
		CCollision::GetInstance()->Process(this, dt, coObjects, true);
	}

	if (state == GREEN_MUSHSHROOM_STATE_RUNNING) {
		vy += ay * dt;
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CGreenMushroom::SetState(int state)
{
	switch (state)
	{
		case GREEN_MUSHSHROOM_STATE_SHOWING: {
			StartShow();
			vy = -MUSHROOM_SHOW_SPEED;
			break;
		}
		case GREEN_MUSHSHROOM_STATE_RUNNING: {
			ay = GREEN_MUSHROOM_GRAVITY;
			vy = 0;
			vx = -GREEN_MUSHROOM_WALKING_SPEED;
			break;
		}
		case GREEN_MUSHSHROOM_STATE_EARNED: {
			point->SetPosition(x, y - GREEN_MUSHSHROOM_POINT_OFFSET_Y);
			point->SetState(POINT_STATE_SHOW);

			LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
			if (dynamic_cast<CPlayScene*>(scene))
			{
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				playScene->GetGameBoard()->AddPoint(GREEN_MUSHSHROOM_POINT);
			}
			break;
		}
		default:
			break;
	}

	CGameObject::SetState(state);
}
