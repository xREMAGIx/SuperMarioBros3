#include "GreenPiranhaPlant.h"

CGreenPiranhaPlant::CGreenPiranhaPlant(float x, float y) :CGameObject(x, y)
{
	ay = 0;
	this->nx = -1;
	SetState(GREEN_PIRANHA_PLANT_STATE_WATING);
	score = new CPoint(x, y);
	score->SetType(POINT_TYPE_100);
}

void CGreenPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GREEN_PIRANHA_PLANT_BBOX_WIDTH / 2;
	top = y - GREEN_PIRANHA_PLANT_BBOX_HEIGHT / 2;
	right = left + GREEN_PIRANHA_PLANT_BBOX_WIDTH;
	bottom = top + GREEN_PIRANHA_PLANT_BBOX_HEIGHT;
}

void CGreenPiranhaPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGreenPiranhaPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CChimney*>(e->obj)) {

	}
}

void CGreenPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GREEN_PIRANHA_PLANT_STATE_DIE)
	{
		isDeleted = true;
		return;
	}

	if (state == GREEN_PIRANHA_PLANT_STATE_JUMP_DIE)
	{
		vy += ay * dt;

		if (GetTickCount64() - dt_die > GREEN_PIRANHA_PLANT_TIME_JUMP_DIE) {
			isDeleted = true;
			return;
		}
	}

	if (state == GREEN_PIRANHA_PLANT_STATE_WATING) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		float marioX, marioY;
		int new_nx, new_ny;

		mario->GetPosition(marioX, marioY);

		if (marioY > y + GREEN_PIRANHA_PLANT_HEIGHT / 2) {
			new_ny = 1;
		}
		else {
			new_ny = -1;
		}

		if (marioX > x + GREEN_PIRANHA_PLANT_WIDTH) {
			new_nx = 1;
		}
		else {
			new_nx = -1;
		}

		this->nx = new_nx;
		this->ny = new_ny;
	}

	//Hidding
	if (state == GREEN_PIRANHA_PLANT_STATE_HIDDEN) {
		if (y > initial_y + GREEN_PIRANHA_PLANT_HEIGHT) {
			y = initial_y + GREEN_PIRANHA_PLANT_HEIGHT;
			SetState(GREEN_PIRANHA_PLANT_STATE_SHOW);
		}
	}
	else {
		if (dt_hide != 0 && GetTickCount64() - dt_hide > GREEN_PIRANHA_PLANT_TIME_HIDE) {
			SetState(GREEN_PIRANHA_PLANT_STATE_HIDDEN);
		}
	}

	//Showing
	if (state == GREEN_PIRANHA_PLANT_STATE_SHOWING) {
		if (y < initial_y - GREEN_PIRANHA_PLANT_HEIGHT) {
			y = initial_y - GREEN_PIRANHA_PLANT_HEIGHT;
			SetState(GREEN_PIRANHA_PLANT_STATE_WATING);
		}
	}
	else {
		if (dt_show != 0 && GetTickCount64() - dt_show > GREEN_PIRANHA_PLANT_TIME_SHOW) {
			SetState(GREEN_PIRANHA_PLANT_STATE_SHOWING);
		}
	}


	if (dt_fire != 0 && GetTickCount64() - dt_fire > GREEN_PIRANHA_PLANT_TIME_SHOW)
	{
		SetState(GREEN_PIRANHA_PLANT_STATE_SHOOT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}


void CGreenPiranhaPlant::Render()
{
	bool flip = false;
	CAnimations* animations = CAnimations::GetInstance();

	switch (state)
	{
	case GREEN_PIRANHA_PLANT_STATE_DIE: {
		current_ani = -1;
		score->Render();
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_JUMP_DIE: {
		flip = true;
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_WATING: {
		current_ani = ID_ANI_GREEN_PIRANHA_PLANT;
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_SHOOT: {
		current_ani = ID_ANI_GREEN_PIRANHA_PLANT;
		break;
	}
	default:
		break;
	}

	animations->Get(current_ani)->Render(x, y, flip);
	// RenderBoundingBox();
}

void CGreenPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_PIRANHA_PLANT_STATE_HIDDEN: {
		vy = GREEN_PIRANHA_PLANT_SPEED;
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_SHOWING: {
		vy = -GREEN_PIRANHA_PLANT_SPEED;
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_SHOW: {
		vy = 0;
		StartShow();
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_WATING: {
		vy = 0;
		StartWaiting();
		StartFire();
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_SHOOT: {
		StopFire();
		StopShow();
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_DIE: {
		StartDie();
		vy = 0;
		score->SetPosition(x, y - GREEN_PIRANHA_PLANT_POINT_OFFSET_Y);
		score->SetState(POINT_STATE_SHOW);
		CBoard* game_board = CBoard::GetInstance();
		game_board->AddPoint(GREEN_PIRANHA_PLANT_POINT_DIE);
		break;
	}
	case GREEN_PIRANHA_PLANT_STATE_JUMP_DIE:
		vx = 0;
		ay = GREEN_PIRANHA_PLANT_GRAVITY;
		vy = -GREEN_PIRANHA_PLANT_JUMP_DIE_SPEED;
		StartDie();
		break;
	default:
		break;
	}
}
