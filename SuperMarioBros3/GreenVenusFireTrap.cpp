#include "GreenVenusFireTrap.h"

CGreenVenusFireTrap::CGreenVenusFireTrap(float x, float y) :CGameObject(x, y)
{
	ay = 0;
	this->nx = -1;
	SetState(GREEN_VENUS_FIRE_TRAP_STATE_WATING);
	current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_DOWN_LEFT;
	score = new CPoint(x, y);
	score->SetType(POINT_TYPE_100);
}

void CGreenVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GREEN_VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
	top = y - GREEN_VENUS_FIRE_TRAP_BBOX_HEIGHT / 2;
	right = left + GREEN_VENUS_FIRE_TRAP_BBOX_WIDTH;
	bottom = top + GREEN_VENUS_FIRE_TRAP_BBOX_HEIGHT;
}

void CGreenVenusFireTrap::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGreenVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CChimney*>(e->obj)) {

	}
}

void CGreenVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GREEN_VENUS_FIRE_TRAP_STATE_DIE)
	{
		isDeleted = true;
		return;
	}

	if (state == GREEN_VENUS_FIRE_TRAP_STATE_JUMP_DIE)
	{
		vy += ay * dt;

		if (GetTickCount64() - dt_die > GREEN_VENUS_FIRE_TRAP_TIME_JUMP_DIE) {
			isDeleted = true;
			return;
		}
	}

	if (state == GREEN_VENUS_FIRE_TRAP_STATE_WATING) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		float marioX, marioY;
		int new_nx, new_ny;

		mario->GetPosition(marioX, marioY);

		if (marioY > y + GREEN_VENUS_FIRE_TRAP_HEIGHT / 2) {
			new_ny = 1;
		}
		else {
			new_ny = -1;
		}

		if (marioX > x + GREEN_VENUS_FIRE_TRAP_WIDTH) {
			new_nx = 1;
		}
		else {
			new_nx = -1;
		}

		this->nx = new_nx;
		this->ny = new_ny;
	}

	//Hidding
	if (state == GREEN_VENUS_FIRE_TRAP_STATE_HIDDEN) {
		if (y > initial_y + GREEN_VENUS_FIRE_TRAP_HEIGHT) {
			y = initial_y + GREEN_VENUS_FIRE_TRAP_HEIGHT;
			SetState(GREEN_VENUS_FIRE_TRAP_STATE_SHOW);
		}
	}
	else {
		if (dt_hide != 0 && GetTickCount64() - dt_hide > GREEN_VENUS_FIRE_TRAP_TIME_HIDE) {
			SetState(GREEN_VENUS_FIRE_TRAP_STATE_HIDDEN);
		}
	}

	//Showing
	if (state == GREEN_VENUS_FIRE_TRAP_STATE_SHOWING) {
		if (y < initial_y - GREEN_VENUS_FIRE_TRAP_HEIGHT) {
			y = initial_y - GREEN_VENUS_FIRE_TRAP_HEIGHT;
			SetState(GREEN_VENUS_FIRE_TRAP_STATE_WATING);
		}
	}
	else {
		if (dt_show != 0 && GetTickCount64() - dt_show > GREEN_VENUS_FIRE_TRAP_TIME_SHOW) {
			SetState(GREEN_VENUS_FIRE_TRAP_STATE_SHOWING);
		}
	}


	if (dt_fire != 0 && GetTickCount64() - dt_fire > GREEN_VENUS_FIRE_TRAP_TIME_SHOW)
	{
		SetState(GREEN_VENUS_FIRE_TRAP_STATE_SHOOT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}


void CGreenVenusFireTrap::Render()
{
	bool flip = false;

	switch (state)
	{
	case GREEN_VENUS_FIRE_TRAP_STATE_DIE: {
		current_ani = -1;
		score->Render();
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_JUMP_DIE: {
		flip = true;
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_WATING: {
		if (nx > 0) {
			//right
			if (ny > 0) {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_DOWN_RIGHT;
			}
			else {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_UP_RIGHT;
			}
		}
		else {
			//left
			if (ny > 0) {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_DOWN_LEFT;
			}
			else {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_UP_LEFT;
			}
		}
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_SHOOT: {
		if (nx > 0) {
			//right
			if (ny > 0) {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_DOWN_RIGHT_SHOOT;
			}
			else {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_UP_RIGHT_SHOOT;
			}
		}
		else {
			//left
			if (ny > 0) {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_DOWN_LEFT_SHOOT;
			}
			else {
				current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_UP_LEFT_SHOOT;
			}
		}
		break;
	}
	default:
		break;
	}

	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(current_ani)->Draw(x, y, flip);
	// RenderBoundingBox();
}

void CGreenVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_VENUS_FIRE_TRAP_STATE_HIDDEN: {
		vy = GREEN_VENUS_FIRE_TRAP_SPEED;
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_SHOWING: {
		vy = -GREEN_VENUS_FIRE_TRAP_SPEED;
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_SHOW: {
		vy = 0;
		StartShow();
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_WATING: {
		vy = 0;
		StartWaiting();
		StartFire();
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_SHOOT: {
		vector<LPGAMEOBJECT>& objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetGameObjects();

		CFireball* fireball = new CFireball(x, y);
		fireball->SetDirection(this->nx, this->ny);

		float fireball_x;
		if (this->nx < 0) {
			fireball_x = x - GREEN_VENUS_FIRE_TRAP_FIRE_BALL_OFFSET_X;
		}
		else {
			fireball_x = x + GREEN_VENUS_FIRE_TRAP_WIDTH + GREEN_VENUS_FIRE_TRAP_FIRE_BALL_OFFSET_X;
		}
		float fireball_y = y + GREEN_VENUS_FIRE_TRAP_FIRE_BALL_OFFSET_Y;

		fireball->SetPosition(fireball_x, fireball_y);
		fireball->SetState(FIREBALL_STATE_THROWN);

		objects.push_back(fireball);

		StopFire();
		StopShow();
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_DIE: {
		StartDie();
		vy = 0;
		score->SetPosition(x, y - GREEN_VENUS_FIRE_TRAP_POINT_OFFSET_Y);
		score->SetState(POINT_STATE_SHOW);
		CBoard* game_board = CBoard::GetInstance();
		game_board->AddPoint(GREEN_VENUS_FIRE_TRAP_POINT_DIE);
		break;
	}
	case GREEN_VENUS_FIRE_TRAP_STATE_JUMP_DIE:
		vx = 0;
		ay = GREEN_VENUS_FIRE_TRAP_GRAVITY;
		vy = -GREEN_VENUS_FIRE_TRAP_JUMP_DIE_SPEED;
		StartDie();
		break;
	default:
		break;
	}
}
