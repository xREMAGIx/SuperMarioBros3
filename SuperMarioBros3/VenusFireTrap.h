#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "Point.h"
#include "Fireball.h"

#define VENUS_FIRE_TRAP_WIDTH 16
#define VENUS_FIRE_TRAP_HEIGHT 32
#define VENUS_FIRE_TRAP_SPEED 0.04f


#define VENUS_FIRE_TRAP_BBOX_WIDTH 16
#define VENUS_FIRE_TRAP_BBOX_HEIGHT 32

#define VENUS_FIRE_TRAP_STATE_HIDDEN 100
#define VENUS_FIRE_TRAP_STATE_SHOWING 200
#define VENUS_FIRE_TRAP_STATE_WATING 300
#define VENUS_FIRE_TRAP_STATE_SHOOT 400
#define VENUS_FIRE_TRAP_STATE_SHOW 500
#define VENUS_FIRE_TRAP_STATE_DIE 600

#define VENUS_FIRE_TRAP_TIME_SHOW 2000
#define VENUS_FIRE_TRAP_TIME_HIDE 3000
#define VENUS_FIRE_TRAP_TIME_FIRE 1000
#define VENUS_FIRE_TRAP_TIME_DIE 700

class CVenusFireTrap : public CGameObject
{
protected:
	DWORD dt_show;
	DWORD dt_hide;
	DWORD dt_fire;
	DWORD dt_die;
	CFireball* fireball;
	CPoint* score;

	float initial_y;
	int current_ani;

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	CVenusFireTrap(float x, float y);
	virtual void SetState(int state);

	void StartShow() {
		dt_hide = 0;
		dt_show = GetTickCount();
		initial_y = y;
	}
	void StopShow() {
		dt_show = 0;
		dt_hide = GetTickCount();
		initial_y = y;
	}
	void StartWaiting() {
		dt_show = 0;
		dt_hide = 0;
	}
	void StartFire() { dt_fire = GetTickCount(); }
	void StopFire() { dt_fire = 0; }

	void StartDie() {
		dt_show = 0;
		dt_hide = 0;
		dt_die = GetTickCount();
	}

};