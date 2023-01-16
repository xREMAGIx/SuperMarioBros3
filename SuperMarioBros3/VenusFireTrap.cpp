#include "VenusFireTrap.h"

CVenusFireTrap::CVenusFireTrap(float x, float y) :CGameObject(x, y)
{
	ay = 0;
	this->nx = -1;
	SetState(VENUS_FIRE_TRAP_STATE_WATING);
	current_ani = ID_SPRITE_VERNUS_FIRE_TRAP_LOOK_DOWN_LEFT;
	score = new CPoint(x, y);
	score->SetType(POINT_TYPE_100);
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
	top = y - VENUS_FIRE_TRAP_BBOX_HEIGHT / 2;
	right = left + VENUS_FIRE_TRAP_BBOX_WIDTH;
	bottom = top + VENUS_FIRE_TRAP_BBOX_HEIGHT;
}

void CVenusFireTrap::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CChimney*>(e->obj)) {
	
	}
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == VENUS_FIRE_TRAP_STATE_DIE)
	{
		isDeleted = true;
		return;
	}
	
	if (state == VENUS_FIRE_TRAP_STATE_JUMP_DIE)
	{
		vy += ay * dt;

		if (GetTickCount64() - dt_die > VENUS_FIRE_TRAP_TIME_JUMP_DIE) {
			isDeleted = true;
			return;
		}
	}

	if (state == VENUS_FIRE_TRAP_STATE_WATING) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		float marioX, marioY;
		int new_nx, new_ny;

		mario->GetPosition(marioX, marioY);

		if (marioY > y + VENUS_FIRE_TRAP_HEIGHT / 2) {
			new_ny = 1;
		}
		else {
			new_ny = -1;
		}

		if (marioX > x + VENUS_FIRE_TRAP_WIDTH) {
			new_nx = 1;
		}
		else {
			new_nx = -1;
		}

		this->nx = new_nx;
		this->ny = new_ny;
	}

	//Hidding
	if (state == VENUS_FIRE_TRAP_STATE_HIDDEN) {
		if (y > initial_y + VENUS_FIRE_TRAP_HEIGHT) {
			y = initial_y + VENUS_FIRE_TRAP_HEIGHT;
			SetState(VENUS_FIRE_TRAP_STATE_SHOW);
		}
	}
	else {
		if (dt_hide != 0 && GetTickCount64() - dt_hide > VENUS_FIRE_TRAP_TIME_HIDE) {
			SetState(VENUS_FIRE_TRAP_STATE_HIDDEN);
		}
	}

	//Showing
	if (state == VENUS_FIRE_TRAP_STATE_SHOWING) {
		if (y < initial_y - VENUS_FIRE_TRAP_HEIGHT) {
			y = initial_y - VENUS_FIRE_TRAP_HEIGHT;
			SetState(VENUS_FIRE_TRAP_STATE_WATING);
		}
	}
	else {
		if (dt_show != 0 && GetTickCount64() - dt_show > VENUS_FIRE_TRAP_TIME_SHOW) {
			SetState(VENUS_FIRE_TRAP_STATE_SHOWING);
		}
	}


	if (dt_fire != 0 && GetTickCount64() - dt_fire > VENUS_FIRE_TRAP_TIME_SHOW)
	{
		SetState(VENUS_FIRE_TRAP_STATE_SHOOT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}


void CVenusFireTrap::Render()
{
	bool flip = false;

	switch (state)
	{
	case VENUS_FIRE_TRAP_STATE_DIE: {
		current_ani = -1;
		score->Render();
		break;
	}
	case VENUS_FIRE_TRAP_STATE_JUMP_DIE: {
		flip = true;
		break;
	}
	case VENUS_FIRE_TRAP_STATE_WATING: {
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
	case VENUS_FIRE_TRAP_STATE_SHOOT: {
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

void CVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case VENUS_FIRE_TRAP_STATE_HIDDEN: {
		vy = VENUS_FIRE_TRAP_SPEED;
		break;
	}
	case VENUS_FIRE_TRAP_STATE_SHOWING: {
		vy = -VENUS_FIRE_TRAP_SPEED;
		break;
	}
	case VENUS_FIRE_TRAP_STATE_SHOW: {
		vy = 0;
		StartShow();
		break;
	}
	case VENUS_FIRE_TRAP_STATE_WATING: {
		vy = 0;
		StartWaiting();
		StartFire();
		break;
	}
	case VENUS_FIRE_TRAP_STATE_SHOOT: {
		vector<LPGAMEOBJECT> &objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetGameObjects();

		CFireball* fireball = new CFireball(x, y);
		fireball->SetDirection(this->nx, this->ny);

		float fireball_x;
		if (this->nx < 0) {
			fireball_x = x - VENUS_FIRE_TRAP_FIRE_BALL_OFFSET_X;
		}
		else {
			fireball_x = x + VENUS_FIRE_TRAP_WIDTH + VENUS_FIRE_TRAP_FIRE_BALL_OFFSET_X;
		}
		float fireball_y = y + VENUS_FIRE_TRAP_FIRE_BALL_OFFSET_Y;

		fireball->SetPosition(fireball_x, fireball_y);
		fireball->SetState(FIREBALL_STATE_THROWN);

		objects.push_back(fireball);

		StopFire();
		StopShow();
		break;
	}
	case VENUS_FIRE_TRAP_STATE_DIE: {
		StartDie();
		vy = 0;
		score->SetPosition(x, y - VENUS_FIRE_TRAP_POINT_OFFSET_Y);
		score->SetState(POINT_STATE_SHOW);
		CBoard* game_board = CBoard::GetInstance();
		game_board->AddPoint(VENUS_FIRE_TRAP_POINT_DIE);
		break;
	}
	case VENUS_FIRE_TRAP_STATE_JUMP_DIE:
		vx = 0;
		ay = VENUS_FIRE_TRAP_GRAVITY;
		vy = -VENUS_FIRE_TRAP_JUMP_DIE_SPEED;
		StartDie();
		break;
	default:
		break;
	}
}
