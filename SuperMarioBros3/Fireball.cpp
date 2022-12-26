#include "Fireball.h"
#include "PlayScene.h"

CFireball::CFireball(float x, float y) :CGameObject(x, y)
{
	state = FIREBALL_STATE_IDLE;
	updateDt = -1;
}

void CFireball::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x - FIREBALL_BBOX_WIDTH / 2;
	top = y - FIREBALL_BBOX_HEIGHT / 2;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

void CFireball::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!dynamic_cast<CMario*>(e->obj)) {
		x += vx * updateDt;
		y += vy * updateDt;
	}
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	updateDt = dt;

	if (state == FIREBALL_STATE_THROWN)
	{
		CGame* game = CGame::GetInstance();
		float camX, camY;
		game->GetCamPos(camX, camY);

		int offset = 32;

		float l = camX - offset,
			t = camY - offset,
			r = camX + game->GetScreenWidth() + offset,
			b = camY + game->GetScreenHeight() + offset;

		if (x < l || x > r || y > b || y < t) {
			state = FIREBALL_STATE_DESTROYED;
		};
	}

	if (state == FIREBALL_STATE_DESTROYED)
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}


void CFireball::Render()
{
	if (state == FIREBALL_STATE_THROWN)
	{
		int aniId = ID_ANI_FIREBALL_THROWN;
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}	
	RenderBoundingBox();
}

void CFireball::SetState(int state)
{
	switch (state)
	{
		case FIREBALL_STATE_THROWN: {
			vx = nx * FIREBALL_SPEED;
			vy = ny * FIREBALL_SPEED;
			break;
		}
		default:
			break;
	}
	CGameObject::SetState(state);
}
