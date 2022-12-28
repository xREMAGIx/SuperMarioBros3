#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SHELL_SCROLL_SPEED 0.08f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 28
#define KOOPA_BBOX_SHELL_HEIGHT 16

#define KOOPA_STATE_WALKING 1
#define KOOPA_STATE_RESPAWN 2
#define KOOPA_STATE_SHELL 3
#define KOOPA_STATE_SHELL_SCROLL 4
#define KOOPA_STATE_SHELL_HOLD 5
#define KOOPA_STATE_DIE 6

#define KOOPA_RESPAWN_START_TIME 5000
#define KOOPA_RESPAWN_TIME 3000

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	bool isRespawning;

	ULONGLONG die_start;
	ULONGLONG respawn_start;
	ULONGLONG respawn_end;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithRedGoomba(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};