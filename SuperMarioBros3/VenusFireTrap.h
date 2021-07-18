#pragma once
#include "GameObject.h"
#include "FireBall.h"

#define VENUS_FIRE_TRAP_WIDTH 16
#define VENUS_FIRE_TRAP_HEIGHT 32
#define VENUS_FIRE_TRAP_SPEED 0.05f


#define VENUS_FIRE_TRAP_BBOX_WIDTH 16
#define VENUS_FIRE_TRAP_BBOX_HEIGHT 32

#define VENUS_FIRE_TRAP_STATE_HIDDEN 100
#define VENUS_FIRE_TRAP_STATE_SHOWING 200
#define VENUS_FIRE_TRAP_STATE_WATING 300
#define VENUS_FIRE_TRAP_STATE_SHOOT 400

#define VENUS_FIRE_TRAP_ANI_LOOK_DOWN_LEFT_SHOOT 0
#define VENUS_FIRE_TRAP_SPRITE_LOOK_DOWN_LEFT 1
#define VENUS_FIRE_TRAP_ANI_LOOK_UP_LEFT_SHOOT 2
#define VENUS_FIRE_TRAP_ANI_LOOK_UP_LEFT 3

#define VENUS_FIRE_TRAP_TIME_SHOW 1000
#define VENUS_FIRE_TRAP_TIME_FIRE 1000

class CVenusFireTrap: public CGameObject
{
	DWORD dt_show;
	DWORD dt_fire;
	CFireball* fireball;

public:
	CVenusFireTrap();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	void StartShow() { dt_show = GetTickCount(); }
	void StopShow() { dt_show = 0; }
	void StartFire() { dt_fire = GetTickCount(); }
	void StopFire() { dt_fire = 0; }
};
