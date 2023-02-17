#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "Point.h"
#include "Fireball.h"
#include "PlayScene.h"

#define GREEN_PIRANHA_PLANT_WIDTH 16
#define GREEN_PIRANHA_PLANT_HEIGHT 32

#define GREEN_PIRANHA_PLANT_SPEED 0.04f
#define GREEN_PIRANHA_PLANT_JUMP_DIE_SPEED	0.3f
#define GREEN_PIRANHA_PLANT_GRAVITY 0.002f

#define GREEN_PIRANHA_PLANT_BBOX_WIDTH 16
#define GREEN_PIRANHA_PLANT_BBOX_HEIGHT 32

#define GREEN_PIRANHA_PLANT_STATE_HIDDEN 100
#define GREEN_PIRANHA_PLANT_STATE_SHOWING 200
#define GREEN_PIRANHA_PLANT_STATE_WATING 300
#define GREEN_PIRANHA_PLANT_STATE_SHOOT 400
#define GREEN_PIRANHA_PLANT_STATE_SHOW 500
#define GREEN_PIRANHA_PLANT_STATE_DIE 600
#define GREEN_PIRANHA_PLANT_STATE_JUMP_DIE 700

#define GREEN_PIRANHA_PLANT_TIME_SHOW 2000
#define GREEN_PIRANHA_PLANT_TIME_HIDE 3000
#define GREEN_PIRANHA_PLANT_TIME_FIRE 1000
#define GREEN_PIRANHA_PLANT_TIME_DIE 700
#define GREEN_PIRANHA_PLANT_TIME_JUMP_DIE 1000

#define GREEN_PIRANHA_PLANT_POINT_OFFSET_Y 32
#define GREEN_PIRANHA_PLANT_FIRE_BALL_OFFSET_X	8
#define GREEN_PIRANHA_PLANT_FIRE_BALL_OFFSET_Y	6

#define GREEN_PIRANHA_PLANT_POINT_DIE 100

class CGreenPiranhaPlant : public CGameObject
{
protected:
	float ay;

	ULONGLONG dt_show;
	ULONGLONG dt_hide;
	ULONGLONG dt_fire;
	ULONGLONG dt_die;

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

	CGreenPiranhaPlant(float x, float y);
	virtual void SetState(int state);

	void StartShow() {
		dt_hide = 0;
		dt_show = GetTickCount64();
		initial_y = y;
	}
	void StopShow() {
		dt_show = 0;
		dt_hide = GetTickCount64();
		initial_y = y;
	}
	void StartWaiting() {
		dt_show = 0;
		dt_hide = 0;
	}
	void StartFire() { dt_fire = GetTickCount64(); }
	void StopFire() { dt_fire = 0; }

	void StartDie() {
		dt_show = 0;
		dt_hide = 0;
		dt_die = GetTickCount64();
	}

};