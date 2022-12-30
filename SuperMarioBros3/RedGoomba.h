#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define RED_GOOMBA_GRAVITY 0.002f
#define RED_GOOMBA_WALKING_SPEED 0.05f
#define RED_GOOMBA_JUMP_DIE_SPEED	0.5f

#define RED_GOOMBA_BBOX_WIDTH 16
#define RED_GOOMBA_BBOX_HEIGHT 14
#define RED_GOOMBA_BBOX_HEIGHT_DIE 7

#define RED_GOOMBA_DIE_TIMEOUT 500
#define GOOMBA_JUMP_DIE_TIMEOUT 1000

#define RED_GOOMBA_STATE_WALKING 100
#define RED_GOOMBA_STATE_DIE 200
#define RED_GOOMBA_STATE_JUMP_DIE 300

class CRedGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CRedGoomba(float x, float y);
	virtual void SetState(int state);
};