#include "Goomba.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);

	fallDetector = new CFallDetector(x, y, 8, 8);

	score = new CPoint(x, y - 16);
	score->SetType(POINT_TYPE_100);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		nx = -nx;
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	bool skipBlockCollide = false;

	if (state == GOOMBA_STATE_DIE)
	{
		score->Update(dt, coObjects);
		if (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) {
			if (score->IsDeleted())
			{
				isDeleted = true;
				return;
			}
		}
	}

	if (state == GOOMBA_STATE_JUMP_DIE)
	{
		skipBlockCollide = true;
		score->Update(dt, coObjects);

		if (GetTickCount64() - die_start > GOOMBA_JUMP_DIE_TIMEOUT) {
			if (score->IsDeleted())
			{
				isDeleted = true;
				return;
			}
		}
	}

	if (state == GOOMBA_STATE_WALKING) {

		int fallDetectorState = fallDetector->GetState();

		if (fallDetectorState != FALL_DETECTOR_STATE_FALL)
		{
			if (fallDetectorState == FALL_DETECTOR_STATE_DETECT) {
				vx = -vx;
				nx = -nx;
			}

			float fallDetectorX, fallDetectorY;
			fallDetectorX = x + nx * (GOOMBA_BBOX_WIDTH + 8);
			fallDetectorY = y - GOOMBA_BBOX_HEIGHT / 2;
			fallDetector->SetPosition(fallDetectorX, fallDetectorY);
			fallDetector->SetState(FALL_DETECTOR_STATE_FALL);
		}

		fallDetector->Update(dt, coObjects);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, skipBlockCollide);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	bool flip = false;

	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
		score->Render();
	}

	if (state == GOOMBA_STATE_JUMP_DIE) {
		flip = true;
		score->Render();
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y, flip);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();

	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			score->SetPosition(x, y - 18);
			score->SetState(POINT_STATE_SHOW);
			if (dynamic_cast<CPlayScene*>(scene))
			{
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				playScene->GetGameBoard()->AddPoint(GOOMBA_POINT_DIE);
			}
			break;
		case GOOMBA_STATE_WALKING: 
			nx = -1;
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_JUMP_DIE: 
			vx = 0;
			vy = -GOOMBA_JUMP_DIE_SPEED;
			die_start = GetTickCount64();
			score->SetType(POINT_TYPE_200);
			score->SetPosition(x, y - 18);
			score->SetState(POINT_STATE_SHOW);
			if (dynamic_cast<CPlayScene*>(scene))
			{
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				playScene->GetGameBoard()->AddPoint(GOOMBA_POINT_JUMP_DIE);
			}
			break;
		default: 
			break;
	}
}
