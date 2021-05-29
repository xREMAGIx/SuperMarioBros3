#pragma once

#include "GameObject.h"

#define MARIO_WIDTH 14
#define MARIO_WALKING_SPEED		0.08f
#define MARIO_JUMP_SPEED_Y		0.4f
#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
//State
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
//Animate
#define MARIO_ANI_IDLE_LEFT			0
#define MARIO_ANI_WALKING_LEFT		1
#define MARIO_ANI_JUMPING_LEFT		2

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

public:
	CMario() : CGameObject()
	{
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
};
