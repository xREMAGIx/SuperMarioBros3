#include "RedKoopa.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "QuestionBlock.h"
#include "debug.h"

CRedKoopa::CRedKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = RED_KOOPA_GRAVITY;
	isHolded = false;
	nx = -1;
	die_start = -1;
	respawn_start = -1;
	SetState(RED_KOOPA_STATE_WALKING);

	fallDetector = new CFallDetector(x, y, 8, 8);
}

void CRedKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == RED_KOOPA_STATE_WALKING)
	{
		left = x - RED_KOOPA_BBOX_WIDTH / 2;
		top = y - RED_KOOPA_BBOX_HEIGHT / 2;
		right = left + RED_KOOPA_BBOX_WIDTH;
		bottom = top + RED_KOOPA_BBOX_HEIGHT;
	}
	else
	{
		left = x - RED_KOOPA_BBOX_WIDTH / 2;
		top = y - RED_KOOPA_BBOX_SHELL_HEIGHT / 2;
		right = left + RED_KOOPA_BBOX_WIDTH;
		bottom = top + RED_KOOPA_BBOX_SHELL_HEIGHT;
	}
}

void CRedKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == RED_KOOPA_STATE_SHELL_SCROLL || state == RED_KOOPA_STATE_SHELL_HOLD) {
		if (dynamic_cast<CGoomba*>(e->obj)) {
			OnCollisionWithGoomba(e);
		}
		else if (dynamic_cast<CRedGoomba*>(e->obj)) {
			OnCollisionWithRedGoomba(e);
		}
		else if (dynamic_cast<CQuestionBlock*>(e->obj)) {
			OnCollisionWithQuestionBlock(e);
		} 
		else if (dynamic_cast<CDeadline*>(e->obj)) {
			isDeleted = true;
			return;
		};
	}

	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		nx = -nx;
	}
}

void CRedKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (state == RED_KOOPA_STATE_SHELL_SCROLL || state == RED_KOOPA_STATE_SHELL_HOLD)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_JUMP_DIE);
		}
	}
}

void CRedKoopa::OnCollisionWithRedGoomba(LPCOLLISIONEVENT e)
{
	CRedGoomba* goomba = dynamic_cast<CRedGoomba*>(e->obj);

	if (state == RED_KOOPA_STATE_SHELL_SCROLL || state == RED_KOOPA_STATE_SHELL_HOLD)
	{
		if (goomba->GetState() != RED_GOOMBA_STATE_DIE)
		{
			goomba->SetState(RED_GOOMBA_STATE_JUMP_DIE);
		}
	}
}

void CRedKoopa::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);

	if (questionBlock->GetState() != QUESTION_BLOCK_STATE_OPENED)
	{
		questionBlock->SetState(QUESTION_BLOCK_STATE_OPENED);
	}
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (isHolded) {
		vy = 0;
		vx = 0;
		ay = 0;
	}
	else {
		ay = RED_KOOPA_GRAVITY;
	}

	if ((state == RED_KOOPA_STATE_DIE))
	{
		isDeleted = true;
		return;
	}

	if ((state == RED_KOOPA_STATE_SHELL) && (GetTickCount64() - respawn_start > RED_KOOPA_RESPAWN_START_TIME))
	{
		SetState(RED_KOOPA_STATE_RESPAWN);
		return;
	}

	if ((state == RED_KOOPA_STATE_RESPAWN) && (GetTickCount64() - respawn_end > RED_KOOPA_RESPAWN_TIME))
	{
		isHolded = false;
		SetState(RED_KOOPA_STATE_WALKING);
		return;
	}

	if (state == RED_KOOPA_STATE_WALKING) {

		int fallDetectorState = fallDetector->GetState();

		if (fallDetectorState != FALL_DETECTOR_STATE_FALL)
		{
			if (fallDetectorState == FALL_DETECTOR_STATE_DETECT) {
				vx = -vx;
				nx = -nx;
			}

			float fallDetectorX, fallDetectorY;
			fallDetectorX = x + nx * (RED_KOOPA_BBOX_WIDTH + 10);
			fallDetectorY = y - 8;
			fallDetector->SetPosition(fallDetectorX, fallDetectorY);
			fallDetector->SetState(FALL_DETECTOR_STATE_FALL);
		}

		fallDetector->Update(dt, coObjects);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CRedKoopa::Render()
{
	int aniId;

	switch (state)
	{
	case RED_KOOPA_STATE_RESPAWN:
	{
		aniId = ID_ANI_RED_KOOPA_RESPAWN;
		break;
	}
	case RED_KOOPA_STATE_SHELL:
	case RED_KOOPA_STATE_SHELL_HOLD:
	{
		aniId = ID_ANI_RED_KOOPA_SHELL;
		break;
	}
	case RED_KOOPA_STATE_SHELL_SCROLL:
	{
		aniId = ID_ANI_RED_KOOPA_SHELL_ROLL;
		break;
	}
	default:
		if (vx > 0)
			aniId = ID_ANI_RED_KOOPA_WALKING_RIGHT;
		else
			aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
		break;
	}
	if (isHolded) {
		renderOrder = 0;
	}
	else {
		renderOrder = 1;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	// RenderBoundingBox();
}

void CRedKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RED_KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (RED_KOOPA_BBOX_HEIGHT - RED_KOOPA_BBOX_SHELL_HEIGHT) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case RED_KOOPA_STATE_SHELL:
		vx = 0;
		vy = 0;
		respawn_start = GetTickCount64();
		break;
	case RED_KOOPA_STATE_RESPAWN:
		respawn_end = GetTickCount64();
		isRespawning = true;
		break;
	case RED_KOOPA_STATE_WALKING:
		if (isRespawning) {
			y += (RED_KOOPA_BBOX_SHELL_HEIGHT - RED_KOOPA_BBOX_HEIGHT) / 2;
			isRespawning = false;
		}
		vx = nx * RED_KOOPA_WALKING_SPEED;
		break;
	}
}
