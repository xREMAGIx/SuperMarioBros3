#include "Koopa.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "QuestionBlock.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y, bool isHaveWing) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	nx = -1;
	die_start = -1;
	respawn_start = -1;
	SetState(KOOPA_STATE_WALKING);

	fallDetector = new CFallDetector(x, y);

	if (isHaveWing) {
		this->isHaveWing = isHaveWing;
		wings = new CWing(x, y);
		wings->SetDirection(1, 0);
		ay = KOOPA_WING_GRAVITY;
	}
	else {
		wings = NULL;
	}
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_SHELL_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_SHELL_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SHELL_SCROLL || state == KOOPA_STATE_SHELL_HOLD) {
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

		if (isHaveWing && e->ny < 0) {
			SetState(KOOPA_STATE_WING_JUMP);
			wings->SetState(WING_STATE_FLAP);
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
		nx = -nx;
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (state == KOOPA_STATE_SHELL_SCROLL || state == KOOPA_STATE_SHELL_HOLD)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_JUMP_DIE);
		}
	}
} 

void CKoopa::OnCollisionWithRedGoomba(LPCOLLISIONEVENT e)
{
	CRedGoomba* goomba = dynamic_cast<CRedGoomba*>(e->obj);

	if (state == KOOPA_STATE_SHELL_SCROLL || state == KOOPA_STATE_SHELL_HOLD)
	{
		if (goomba->GetState() != RED_GOOMBA_STATE_DIE)
		{
			goomba->SetState(RED_GOOMBA_STATE_JUMP_DIE);
		}
	}
}

void CKoopa::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);

	if (questionBlock->GetState() != QUESTION_BLOCK_STATE_OPENED)
	{
		questionBlock->SetState(QUESTION_BLOCK_STATE_OPENED);
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (isHolded) {
		vy = 0;
		vx = 0;
	}

	if (state == KOOPA_STATE_JUMP_DIE) {
		if (GetTickCount64() - die_start > KOOPA_JUMP_DIE_TIMEOUT) {
			isDeleted = true;
			return;
		}
	}

	if ((state == KOOPA_STATE_DIE))
	{
		isDeleted = true;
		return;
	}

	if ((state == KOOPA_STATE_SHELL) && (GetTickCount64() - respawn_start > KOOPA_RESPAWN_START_TIME))
	{
		SetState(KOOPA_STATE_RESPAWN);
		return;
	}

	if ((state == KOOPA_STATE_RESPAWN) && (GetTickCount64() - respawn_end > KOOPA_RESPAWN_TIME))
	{
		isHolded = false;
		SetState(KOOPA_STATE_WALKING);
		return;
	}

	if (state == KOOPA_STATE_WALKING) {

		int fallDetectorState = fallDetector->GetState();

		if (fallDetectorState != FALL_DETECTOR_STATE_FALL)
		{
			if (fallDetectorState == FALL_DETECTOR_STATE_DETECT) {
				vx = -vx;
				nx = -nx;
			}

			float fallDetectorX, fallDetectorY;
			fallDetectorX = x + nx * (KOOPA_BBOX_WIDTH + KOOPA_FALL_DETECTOR_OFFSET_X);
			fallDetectorY = y - KOOPA_FALL_DETECTOR_OFFSET_Y;
			fallDetector->SetPosition(fallDetectorX, fallDetectorY);
			fallDetector->SetState(FALL_DETECTOR_STATE_FALL);
		}

		fallDetector->Update(dt, coObjects);
	}

	if (isHaveWing) {
		wings->SetPosition(x + KOOPA_WINGS_X, y + KOOPA_WINGS_Y);
		wings->Update(dt, coObjects);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopa::Render()
{
	int aniId = -1;
	bool flip = false;

	switch (state)
	{
	case KOOPA_STATE_RESPAWN:
	{
		aniId = ID_ANI_KOOPA_RESPAWN;
		break;
	}
	case KOOPA_STATE_SHELL:
	case KOOPA_STATE_SHELL_HOLD:
	{
		aniId = ID_ANI_KOOPA_SHELL;
		break;
	}
	case KOOPA_STATE_SHELL_SCROLL:
	{
		aniId = ID_ANI_KOOPA_SHELL_ROLL;
		break;
	}
	case KOOPA_STATE_JUMP_DIE:
	{
		flip = true;
		aniId = ID_ANI_KOOPA_SHELL;
		break;
	}
	default:
		if (vx > 0)
			aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		else
			aniId = ID_ANI_KOOPA_WALKING_LEFT;
		break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (isHaveWing) {
		wings->Render();
	}
	// RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case KOOPA_STATE_DIE:
			die_start = GetTickCount64();
			y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_SHELL_HEIGHT) / 2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case KOOPA_STATE_SHELL: 
			vx = 0;
			vy = 0;
			respawn_start = GetTickCount64();
			break;
		case KOOPA_STATE_RESPAWN:
			respawn_end = GetTickCount64();
			isRespawning = true;
			break;
		case KOOPA_STATE_WALKING:
			if (isRespawning) {
				y += (KOOPA_BBOX_SHELL_HEIGHT - KOOPA_BBOX_HEIGHT) / 2;
				isRespawning = false;
			}
			vx = nx * KOOPA_WALKING_SPEED;
			ay = KOOPA_GRAVITY;
			break;
		case KOOPA_STATE_WING_JUMP:
			vy = -KOOPA_WING_JUMP_SPEED;
			wings->SetState(WING_STATE_FLAP);
			break;
		default:
			break;
	}
}
