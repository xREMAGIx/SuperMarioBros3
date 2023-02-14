#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "FallDetector.h"
#include "Wing.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SHELL_SCROLL_SPEED 0.08f
#define KOOPA_JUMP_DIE_SPEED	0.5f
#define KOOPA_WING_JUMP_SPEED	0.3f
#define KOOPA_WING_GRAVITY 0.001f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 28
#define KOOPA_BBOX_SHELL_HEIGHT 16

#define KOOPA_STATE_WALKING 1
#define KOOPA_STATE_RESPAWN 2
#define KOOPA_STATE_SHELL 3
#define KOOPA_STATE_SHELL_SCROLL 4
#define KOOPA_STATE_SHELL_HOLD 5
#define KOOPA_STATE_DIE 6
#define KOOPA_STATE_JUMP_DIE 7
#define KOOPA_STATE_WING_JUMP 8

#define KOOPA_RESPAWN_START_TIME 5000
#define KOOPA_RESPAWN_TIME 3000
#define KOOPA_JUMP_DIE_TIMEOUT 1000

#define KOOPA_POINT_JUMP_DIE 200

#define KOOPA_FALL_DETECTOR_OFFSET_X 10
#define KOOPA_FALL_DETECTOR_OFFSET_Y	8

#define KOOPA_WINGS_X	4
#define KOOPA_WINGS_Y	-4

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	bool isRespawning;
	bool isHolded;

	bool isHaveWing = false;

	CFallDetector* fallDetector;

	ULONGLONG die_start;
	ULONGLONG respawn_start;
	ULONGLONG respawn_end;
	CWing* wings;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	virtual int IsCollidable() { return ((state != KOOPA_STATE_DIE) && (state != KOOPA_STATE_JUMP_DIE)); };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithRedGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y, bool isHaveWing = false);
	virtual void SetState(int state);

	bool GetIsHolded() {
		return this->isHolded;
	}
	void SetIsHolded(bool isHolded) {
		this->isHolded = isHolded;
	};

	bool GetIsHaveWing() { return isHaveWing; }
	void SetIsHaveWing(bool isHaveWing) { this->isHaveWing = isHaveWing; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};