#pragma once

#include "GameObject.h"

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

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  16
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_DIE_TIME 3000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	DWORD dt_die;
	int current_ani;

public:

	CMario() : CGameObject()
	{
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		current_ani = MARIO_ANI_IDLE_LEFT;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);

	int GetUntouchable() { return this->untouchable; };

	int GetLevel() { return this->level; };
	void SetLevel(int l) { level = l;  
		if (l == MARIO_LEVEL_BIG) {
			this->y += -MARIO_BIG_BBOX_HEIGHT + 1;
		}
	}

	void SetCurrentAni(int current_ani) {
		this->current_ani = current_ani;
	}

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartDie() { dt_die = GetTickCount(); }

};
