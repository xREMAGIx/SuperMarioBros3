#pragma once

#include "GameObject.h"
#include "MarioTail.h"
#include "MarioFireball.h"

#define MARIO_WIDTH 16
#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.3f
#define MARIO_GRAVITY			0.001f
#define MARIO_DEL_ACCEL_SPEED		0.01f
#define MARIO_ACCEL_SPEED		0.005f

#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

//State
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_JUMP_LEFT		400
#define MARIO_STATE_JUMP_RIGHT		500
#define MARIO_STATE_DIE				600
#define MARIO_STATE_RUNNING			700
#define MARIO_STATE_HOLDING			800
#define MARIO_STATE_KICK		900
#define MARIO_STATE_DOWN		1000
#define MARIO_STATE_ATTACK		1100
#define MARIO_STATE_SLOW_FLYING		1200
#define MARIO_STATE_FLYING		1300
#define MARIO_STATE_GO_CHIMNEY		1400

//Animate
#define MARIO_ANI_IDLE_LEFT			0
#define MARIO_ANI_WALKING_LEFT		1
#define MARIO_ANI_JUMPING_LEFT		2
#define MARIO_ANI_BIG_IDLE_LEFT			3
#define MARIO_ANI_BIG_WALKING_LEFT		4
#define MARIO_ANI_BIG_JUMPING_LEFT		5
#define MARIO_ANI_DIE		6
#define MARIO_ANI_RUNNING		7
#define MARIO_ANI_HOLDING		8
#define MARIO_ANI_KICK		9
#define MARIO_ANI_BIG_DOWN		10
#define MARIO_ANI_BIG_RUNNING		11
#define MARIO_ANI_FLYING		12
#define MARIO_ANI_BIG_FLYING		13

#define MARIO_ANI_TAIL_IDLE_LEFT		14
#define MARIO_ANI_TAIL_WALKING_LEFT		15
#define MARIO_ANI_TAIL_JUMPING_LEFT		16
#define MARIO_ANI_TAIL_DOWN		17
#define MARIO_ANI_TAIL_RUNNING		18
#define MARIO_ANI_TAIL_FLYING		19
#define MARIO_ANI_TAIL_ATTACK		20

#define MARIO_ANI_FIRE_IDLE_LEFT		21
#define MARIO_ANI_FIRE_WALKING_LEFT		22
#define MARIO_ANI_FIRE_JUMPING_LEFT		23
#define MARIO_ANI_FIRE_DOWN		24
#define MARIO_ANI_FIRE_RUNNING		25
#define MARIO_ANI_FIRE_FLYING		26
#define MARIO_ANI_FIRE_KICKING		27
#define MARIO_ANI_FIRE_ATTACK		28

//Level
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TAIL		3
#define	MARIO_LEVEL_FIRE		4

#define MARIO_BIG_BBOX_WIDTH  16
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  16
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_DIE_TIME 3000
#define MARIO_ATTACK_TIME 500
#define MARIO_GO_CHIMNEY_TIME 500


class CMario : public CGameObject
{
	int level;
	int untouchable;

	DWORD untouchable_start;
	DWORD dt_die;
	DWORD dt_attack;
	DWORD dt_go_chimney;

	int current_ani;
	int down;
	int up;

	float deflect_gravity = 0.0f;

	CMarioTail* tailAttack;
	CMarioFireball* fireball;

public:

	CMario() : CGameObject()
	{
		dt_attack = 0;
		dt_die = 0;
		dt_go_chimney = 0;
		down = up = 0;
		untouchable = 0;
		untouchable_start = 0;
		level = MARIO_LEVEL_SMALL;
		current_ani = MARIO_ANI_IDLE_LEFT;
		tailAttack = new CMarioTail();
		fireball = new CMarioFireball();
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);

	int GetUntouchable() { return this->untouchable; };

	CMarioTail* GetTail() { return this->tailAttack; };

	int GetLevel() { return this->level; };
	void SetLevel(int l) { level = l;  
		if (l != MARIO_LEVEL_SMALL) {
			y += MARIO_SMALL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT - 1;
		}
	}

	void SetDownLevel() {
		switch (level)
		{
		case MARIO_LEVEL_FIRE:
		case MARIO_LEVEL_TAIL: {
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
			break;
		}
		case MARIO_LEVEL_BIG: {
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
			break;
		}
		default:
			SetState(MARIO_STATE_DIE);
			break;
		}
	}

	void SetCurrentAni(int current_ani) {
		this->current_ani = current_ani;
	}

	void SetUp(int up) {
		this->up = up;
	}

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartDie() { dt_die = GetTickCount(); }
	void StartAttack() { dt_attack = GetTickCount(); }
	void StopAttack() { dt_attack = 0; }

	void SetSlowFly() { deflect_gravity = MARIO_GRAVITY * 5 / 6; }
	void ResetSlowFly() { deflect_gravity = 0; }

	void StartGoChimney()  { dt_go_chimney = GetTickCount(); } 
};
