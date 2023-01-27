#include "PSwitch.h"
#include "PlayScene.h"

CPSwitch::CPSwitch(float x, float y) :CGameObject(x, y)
{
	die_start = -1;
	ay = P_SWITCH_GRAVITY;
	state = P_SWITCH_STATE_IDLE;
}

void CPSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == P_SWITCH_STATE_PUSHED)
	{
		left = x - P_SWITCH_BBOX_WIDTH / 2;
		top = y - P_SWITCH_BBOX_HEIGHT_PUSHED / 2;
		right = left + P_SWITCH_BBOX_WIDTH;
		bottom = top + P_SWITCH_BBOX_HEIGHT_PUSHED;
	}
	else
	{
		left = x - P_SWITCH_BBOX_WIDTH / 2;
		top = y - P_SWITCH_BBOX_HEIGHT / 2;
		right = left + P_SWITCH_BBOX_WIDTH;
		bottom = top + P_SWITCH_BBOX_HEIGHT;
	}
}

void CPSwitch::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CPSwitch::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		nx = -nx;
		vx = -vx;
	}
}

void CPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == P_SWITCH_STATE_PUSHED)
	{
		if (GetTickCount64() - die_start > P_SWITCH_PUSHED_TIMEOUT) {
			isDeleted = true;
			return;
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPSwitch::Render()
{
	int aniId = ID_ANI_P_SWITCH_IDLE;

	if (state == P_SWITCH_STATE_PUSHED)
	{
		aniId = ID_ANI_P_SWITCH_PUSHED;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPSwitch::SetState(int state)
{
	CGameObject::SetState(state);
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();

	switch (state)
	{
	case P_SWITCH_STATE_PUSHED:
		die_start = GetTickCount64();
		ay = 0;
		y += (P_SWITCH_BBOX_HEIGHT - P_SWITCH_BBOX_HEIGHT_PUSHED) / 2;
		vx = 0;
		vy = 0;

		if (dynamic_cast<CPlayScene*>(scene))
		{
			vector<LPGAMEOBJECT>& objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetGameObjects();

			for (size_t i = 1; i < objects.size(); i++)
			{
				if (dynamic_cast<CBrick*>(objects[i])) {
					objects[i]->SetState(BRICK_STATE_TURNED);
				}
			}
		}
		break;
	default:
		break;
	}
}
