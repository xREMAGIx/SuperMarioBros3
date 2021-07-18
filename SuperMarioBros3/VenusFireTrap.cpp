#include "Game.h"
#include "Goomba.h"
#include "VenusFireTrap.h"

CVenusFireTrap::CVenusFireTrap()
{
	SetState(VENUS_FIRE_TRAP_STATE_SHOWING);
	SetDirection(-1);
	fireball = new CFireball();
	//score = new CPoint();
	//score->SetPointId(POINT_ID_100);
}


void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x; 
	top = y;
	right = x + VENUS_FIRE_TRAP_BBOX_WIDTH;
	bottom = y + VENUS_FIRE_TRAP_BBOX_HEIGHT;
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (state == VENUS_FIRE_TRAP_STATE_SHOWING) {
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

	if (state == VENUS_FIRE_TRAP_STATE_HIDDEN) {
		//score->Update(dt, coObjects);
	}
	
	if (dt_fire != 0 && GetTickCount() - dt_fire > VENUS_FIRE_TRAP_TIME_SHOW)
	{
		SetState(VENUS_FIRE_TRAP_STATE_SHOOT);
		fireball->SetDirection(nx);
		fireball->SetDirectionY(ny);

		int fireball_x;
		if (nx < 0) {
			fireball_x = x - 8;
		}
		else {
			fireball_x = x + VENUS_FIRE_TRAP_WIDTH + 8;
		}
		int fireball_y = y + 6;

		fireball->SetPosition(fireball_x, fireball_y);
		fireball->SetState(FIREBALL_STATE_THROWN);

		StopFire();
	}

	if (fireball->GetState() == FIREBALL_STATE_THROWN) {
		fireball->Update(dt, coObjects);
	}
}

void CVenusFireTrap::Render()
{
	int ani = VENUS_FIRE_TRAP_SPRITE_LOOK_DOWN_LEFT;

	switch (state)
	{
		case VENUS_FIRE_TRAP_STATE_SHOWING: {
			if (ny > 0) {
				ani = VENUS_FIRE_TRAP_SPRITE_LOOK_DOWN_LEFT;
			}
			else {
				ani = VENUS_FIRE_TRAP_ANI_LOOK_UP_LEFT;
			}
			break;
		}
		case VENUS_FIRE_TRAP_STATE_SHOOT: {
			if (ny > 0) {
				ani = VENUS_FIRE_TRAP_ANI_LOOK_DOWN_LEFT_SHOOT;
			}
			else {
				ani = VENUS_FIRE_TRAP_ANI_LOOK_UP_LEFT_SHOOT;
			}
			break;
		}
		default:
			break;
	}

	if (fireball->GetState() == FIREBALL_STATE_THROWN) {
		fireball->Render();
	}

	animation_set->at(ani)->Render(x, y, nx, 255);
}

void CVenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case VENUS_FIRE_TRAP_STATE_HIDDEN: {
			y -= VENUS_FIRE_TRAP_BBOX_HEIGHT;
			StartShow();
			break;
		} 
		case VENUS_FIRE_TRAP_STATE_SHOWING: {
			StartFire();
			break;
		}
		default: 
			break;
	}
}
