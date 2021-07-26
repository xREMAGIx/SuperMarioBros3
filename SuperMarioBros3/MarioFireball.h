#pragma once
#include "GameObject.h"

#define MARIO_FIREBALL_WIDTH 11
#define MARIO_FIREBALL_HEIGHT 11

#define MARIO_FIREBALL_BBOX_WIDTH 11
#define MARIO_FIREBALL_BBOX_HEIGHT 11

#define MARIO_FIREBALL_SPEED 0.07f
#define MARIO_FIREBALL_GRAVITY 0.001f

#define MARIO_FIREBALL_STATE_THROWN 100
#define MARIO_FIREBALL_STATE_DESTROY 200
#define MARIO_FIREBALL_STATE_DESTROYED 300

#define MARIO_FIREBALL_ANI_SET_ID 135

#define MARIO_FIREBALL_ANI_THROWN 0
#define MARIO_FIREBALL_ANI_DESTROYED 1

#define MARIO_FIREBALL_DESTROYED_TIME 500


class CMarioFireball : public CGameObject
{
	DWORD dt_destroy = 0;

public:
	CMarioFireball();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);

	void StartSmoke() { dt_destroy = GetTickCount(); }
	void StopSmoke() { dt_destroy = 0; }
};
