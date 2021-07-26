#include "Game.h"
#include "Goomba.h"
#include "VenusFireTrap.h"

CVenusFireTrap::CVenusFireTrap()
{
	SetDirection(-1);
	fireball = new CFireball();
	SetState(VENUS_FIRE_TRAP_STATE_WATING);
	current_ani = VENUS_FIRE_TRAP_SPRITE_LOOK_DOWN_LEFT;
	score = new CPoint();
	score->SetPointId(POINT_ID_100);
}


void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x; 
	top = y;
	right = x + VENUS_FIRE_TRAP_BBOX_WIDTH;
	bottom = y + VENUS_FIRE_TRAP_BBOX_HEIGHT;

	if ((dt_die != 0 && GetTickCount() - dt_die > VENUS_FIRE_TRAP_TIME_DIE))
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += dy;

	if (state == VENUS_FIRE_TRAP_STATE_WATING) {
		LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
		CMario* mario = ((CPlayScene*)scene)->GetPlayer();

		int new_nx, new_ny;
		if (mario->y > y + VENUS_FIRE_TRAP_HEIGHT / 2) {
			new_ny = 1;
		}
		else {
			new_ny = -1;
		}

		if (mario->x > x + VENUS_FIRE_TRAP_WIDTH) {
			new_nx = 1;
		}
		else {
			new_nx = -1;
		}

		SetDirection(new_nx);
		SetDirectionY(new_ny);
	}

	//Hidding
	if (state == VENUS_FIRE_TRAP_STATE_HIDDEN) {
		if (y > initial_y + VENUS_FIRE_TRAP_HEIGHT + 3) {
			y = initial_y + VENUS_FIRE_TRAP_HEIGHT + 3;
			SetState(VENUS_FIRE_TRAP_STATE_SHOW);
		}
	}
	else {
		if (dt_hide != 0 && GetTickCount() - dt_hide > VENUS_FIRE_TRAP_TIME_HIDE) {
			SetState(VENUS_FIRE_TRAP_STATE_HIDDEN);
		}
	}

	//Showing
	if (state == VENUS_FIRE_TRAP_STATE_SHOWING) {
		if (y < initial_y - VENUS_FIRE_TRAP_HEIGHT) {
			y = initial_y - VENUS_FIRE_TRAP_HEIGHT - 3;
			SetState(VENUS_FIRE_TRAP_STATE_WATING);
		}
	}
	else {
		if (dt_show != 0 && GetTickCount() - dt_show > VENUS_FIRE_TRAP_TIME_SHOW) {
			SetState(VENUS_FIRE_TRAP_STATE_SHOWING);
		}
	}

	if (state == VENUS_FIRE_TRAP_STATE_DIE) {
		score->Update(dt, coObjects);
	}

	if (dt_fire != 0 && GetTickCount() - dt_fire > VENUS_FIRE_TRAP_TIME_SHOW)
	{
		SetState(VENUS_FIRE_TRAP_STATE_SHOOT);
		fireball->SetDirection(this->nx);
		fireball->SetDirectionY(this->ny);

		float fireball_x;
		if (this->nx < 0) {
			fireball_x = x - 8;
		}
		else {
			fireball_x = x + VENUS_FIRE_TRAP_WIDTH + 8;
		}
		float fireball_y = y + 6;

		fireball->SetPosition(fireball_x, fireball_y);
		fireball->SetState(FIREBALL_STATE_THROWN);

		StopFire();
		StopShow();
	}

	if (fireball->GetState() == FIREBALL_STATE_THROWN) {
		fireball->Update(dt, coObjects);
	}
}

void CVenusFireTrap::Render()
{
	switch (state)
	{
		case VENUS_FIRE_TRAP_STATE_DIE: {
			current_ani = -1;
			score->Render();
			break;
		}
		case VENUS_FIRE_TRAP_STATE_WATING: {
			if (ny > 0) {
				current_ani = VENUS_FIRE_TRAP_SPRITE_LOOK_DOWN_LEFT;
			}
			else {
				current_ani = VENUS_FIRE_TRAP_ANI_LOOK_UP_LEFT;
			}
			break;
		}
		case VENUS_FIRE_TRAP_STATE_SHOOT: {
			if (ny > 0) {
				current_ani = VENUS_FIRE_TRAP_ANI_LOOK_DOWN_LEFT_SHOOT;
			}
			else {
				current_ani = VENUS_FIRE_TRAP_ANI_LOOK_UP_LEFT_SHOOT;
			}
			break;
		}
		default:
			break;
	}

	if (fireball->GetState() == FIREBALL_STATE_THROWN) {
		fireball->Render();
	}
	if (current_ani != -1) {
		animation_set->at(current_ani)->Render(x, y, nx, 255);
	}
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
		case VENUS_FIRE_TRAP_STATE_DIE: {
			vy = 0;
			score->SetPosition(x, y - 32);
			score->SetState(POINT_STATE_SHOW);
			CBoard* game_board = CBoard::GetInstance();
			game_board->AddPoint(100);
			StartDie();
			break;
		}
		default: 
			break;
	}
}
