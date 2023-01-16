#include "RedGoomba.h"
#include "PlayScene.h"


CRedGoomba::CRedGoomba(float x, float y, bool isHaveWing) :CGameObject(x, y)
{
	nx = -1;
	this->ax = 0;
	this->ay = RED_GOOMBA_GRAVITY;
	die_start = -1;
	SetState(RED_GOOMBA_STATE_WALKING);

	fallDetector = new CFallDetector(x, y);
	score = new CPoint(x, y - RED_GOOMBA_POINT_OFFSET_Y);
	score->SetType(POINT_TYPE_100);

	if (isHaveWing) {
		this->isHaveWing = isHaveWing;
		leftWing = new CWing(x, y);
		rightWing = new CWing(x + RED_GOOMBA_BBOX_WIDTH, y);
		rightWing->SetDirection(1, 0);
	}
	else {
		leftWing = NULL;
		rightWing = NULL;
	}
}

void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == RED_GOOMBA_STATE_DIE)
	{
		left = x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT;
	}
}

void CRedGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRedGoomba*>(e->obj)) return;
	if (dynamic_cast<CDeadline*>(e->obj)) {
		isDeleted = true;
		return;
	};

	if (e->ny != 0)
	{
		vy = 0;

		if (isHaveWing && e->ny < 0) {
			if (jumpCount == RED_GOOMBA_MAXIMUM_JUMP_COUNT) {
				float marioX, marioY;
				CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

				mario->GetPosition(marioX, marioY);
				
				if (nx > 0) {
					if (marioX < x - RED_GOOMBA_BBOX_WIDTH / 2) {
						vx = -vx;
						nx = -nx;
					}
				}
				else {
					if (marioX > x + RED_GOOMBA_BBOX_WIDTH / 2) {
						vx = -vx;
						nx = -nx;
					}
				}
				
				SetState(RED_GOOMBA_STATE_JUMP_BIG);
				jumpCount = 0;
				leftWing->SetState(WING_STATE_IDLE);
				rightWing->SetState(WING_STATE_IDLE);
			}
			else if (jumpCount == RED_GOOMBA_FIRST_MID_JUMP_COUNT || jumpCount == RED_GOOMBA_SECOND_MID_JUMP_COUNT) {
				SetState(RED_GOOMBA_STATE_JUMP_SMALL);
				jumpCount++;
			}
			else {
				jumpCount++;
			}
		}

	}
	else if (e->nx != 0)
	{
		nx = -nx;
		vx = -vx;
	}
}

void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	bool skipBlockCollide = false;
	int fallDetectorState = fallDetector->GetState();

	switch (state)
	{
		case RED_GOOMBA_STATE_WALKING:

			if (fallDetectorState != FALL_DETECTOR_STATE_FALL)
			{
				if (fallDetectorState == FALL_DETECTOR_STATE_DETECT) {
					nx = -nx;
					vx = -vx;
				}

				float fallDetectorX, fallDetectorY;
				fallDetectorX = x + nx * (RED_GOOMBA_BBOX_WIDTH + FALL_DETECTOR_CELL_WIDTH);
				fallDetectorY = y - RED_GOOMBA_BBOX_HEIGHT / 2;
				fallDetector->SetPosition(fallDetectorX, fallDetectorY);
				fallDetector->SetState(FALL_DETECTOR_STATE_FALL);
			}

			fallDetector->Update(dt, coObjects);
			break;
		case RED_GOOMBA_STATE_DIE:
			score->Update(dt, coObjects);
			if (GetTickCount64() - die_start > RED_GOOMBA_DIE_TIMEOUT) {
				if (score->IsDeleted())
				{
					isDeleted = true;
					return;
				}
			}
			break;
		case RED_GOOMBA_STATE_JUMP_DIE:
			skipBlockCollide = true;
			score->Update(dt, coObjects);

			if (GetTickCount64() - die_start > RED_GOOMBA_JUMP_DIE_TIMEOUT) {
				if (score->IsDeleted())
				{
					isDeleted = true;
					return;
				}
			}
			break;
		default:
			break;
	}

	if (isHaveWing) {
		leftWing->SetPosition(x + RED_GOOMBA_LEFT_WING_X, y + RED_GOOMBA_LEFT_WING_Y);
		rightWing->SetPosition(x + RED_GOOMBA_RIGHT_WING_X, y + RED_GOOMBA_RIGHT_WING_Y);
		leftWing->Update(dt, coObjects);
		rightWing->Update(dt, coObjects);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, skipBlockCollide);
}


void CRedGoomba::Render()
{
	int aniId = ID_ANI_RED_GOOMBA_WALKING;
	bool flip = false;

	if (state == RED_GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_RED_GOOMBA_DIE;
		score->Render();
	}

	if (state == RED_GOOMBA_STATE_JUMP_DIE) {
		flip = true;
		score->Render();
	}

	if (isHaveWing) {
		leftWing->Render();
		rightWing->Render();
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y, flip);
	// RenderBoundingBox();
}

void CRedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();

	switch (state)
	{
		case RED_GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (RED_GOOMBA_BBOX_HEIGHT - RED_GOOMBA_BBOX_HEIGHT_DIE) / 2;
			vx = 0;
			vy = 0;
			ay = 0;
			score->SetPosition(x, y - RED_GOOMBA_POINT_OFFSET_Y);
			score->SetState(POINT_STATE_SHOW);
			if (dynamic_cast<CPlayScene*>(scene))
			{
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				playScene->GetGameBoard()->AddPoint(RED_GOOMBA_POINT_DIE);
			}
			break;
		case RED_GOOMBA_STATE_JUMP_DIE:
			vx = 0;
			vy = -RED_GOOMBA_JUMP_DIE_SPEED;
			die_start = GetTickCount64();
			score->SetType(POINT_TYPE_200);
			score->SetPosition(x, y - RED_GOOMBA_POINT_OFFSET_Y);
			score->SetState(POINT_STATE_SHOW);
			if (dynamic_cast<CPlayScene*>(scene))
			{
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				playScene->GetGameBoard()->AddPoint(RED_GOOMBA_POINT_JUMP_DIE);
			}
			break;
		case RED_GOOMBA_STATE_WALKING:
			vx = nx * RED_GOOMBA_WALKING_SPEED;
			break;
		case RED_GOOMBA_STATE_JUMP_SMALL:
			vy = -RED_GOOMBA_JUMP_SPEED;
			leftWing->SetState(WING_STATE_FLAP);
			rightWing->SetState(WING_STATE_FLAP);
			break;
		case RED_GOOMBA_STATE_JUMP_BIG:
			vy = -RED_GOOMBA_JUMP_SPEED * 3;
			leftWing->SetState(WING_STATE_FLAP);
			rightWing->SetState(WING_STATE_FLAP);
			break;
		default:
			break;
	}
}
