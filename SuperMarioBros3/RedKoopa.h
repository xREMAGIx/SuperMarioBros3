#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "FallDetector.h"

#define RED_KOOPA_GRAVITY 0.002f
#define RED_KOOPA_WALKING_SPEED 0.05f
#define RED_KOOPA_SHELL_SCROLL_SPEED 0.08f

#define RED_KOOPA_BBOX_WIDTH 16
#define RED_KOOPA_BBOX_HEIGHT 28
#define RED_KOOPA_BBOX_SHELL_HEIGHT 16

#define RED_KOOPA_STATE_WALKING 1
#define RED_KOOPA_STATE_RESPAWN 2
#define RED_KOOPA_STATE_SHELL 3
#define RED_KOOPA_STATE_SHELL_SCROLL 4
#define RED_KOOPA_STATE_SHELL_HOLD 5
#define RED_KOOPA_STATE_DIE 6

#define RED_KOOPA_RESPAWN_START_TIME 5000
#define RED_KOOPA_RESPAWN_TIME 3000

class CRedKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	bool isRespawning;
	bool isHolded;

	CFallDetector* fallDetector;

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
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);

public:
	CRedKoopa(float x, float y);
	virtual void SetState(int state);

	bool GetIsHolded() {
		return this->isHolded;
	}
	void SetIsHolded(bool isHolded) {
		this->isHolded = isHolded;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};