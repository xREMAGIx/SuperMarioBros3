#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define P_SWITCH_GRAVITY 0.002f

#define P_SWITCH_BBOX_WIDTH 15
#define P_SWITCH_BBOX_HEIGHT 16
#define P_SWITCH_BBOX_HEIGHT_PUSHED 7

#define P_SWITCH_PUSHED_TIMEOUT 500

#define P_SWITCH_STATE_IDLE 1
#define P_SWITCH_STATE_PUSHED 2

#define	P_SWITCH_POINT_OFFSET_Y	16

class CPSwitch : public CGameObject
{
protected:
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != P_SWITCH_STATE_PUSHED; };
	virtual int IsBlocking() { return state != P_SWITCH_STATE_PUSHED; };

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPSwitch(float x, float y);
	virtual void SetState(int state);
};