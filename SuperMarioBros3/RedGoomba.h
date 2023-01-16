#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "FallDetector.h"
#include "Wing.h"
#include "Point.h"

#define RED_GOOMBA_GRAVITY 0.002f
#define RED_GOOMBA_WALKING_SPEED 0.05f
#define RED_GOOMBA_JUMP_DIE_SPEED	0.5f
#define RED_GOOMBA_JUMP_SPEED	0.15f

#define RED_GOOMBA_BBOX_WIDTH 16
#define RED_GOOMBA_BBOX_HEIGHT 14
#define RED_GOOMBA_BBOX_HEIGHT_DIE 7

#define RED_GOOMBA_DIE_TIMEOUT 500
#define RED_GOOMBA_JUMP_DIE_TIMEOUT 1000

#define RED_GOOMBA_STATE_WALKING 1
#define RED_GOOMBA_STATE_DIE 2
#define RED_GOOMBA_STATE_JUMP_DIE 3
#define RED_GOOMBA_STATE_JUMP_SMALL 4
#define RED_GOOMBA_STATE_JUMP_BIG 5

#define RED_GOOMBA_LEFT_WING_X -(RED_GOOMBA_BBOX_WIDTH/2)
#define RED_GOOMBA_LEFT_WING_Y -(RED_GOOMBA_BBOX_HEIGHT/2)
#define RED_GOOMBA_RIGHT_WING_X (RED_GOOMBA_BBOX_WIDTH/2)
#define RED_GOOMBA_RIGHT_WING_Y -(RED_GOOMBA_BBOX_HEIGHT/2)

#define RED_GOOMBA_POINT_DIE 100
#define RED_GOOMBA_POINT_JUMP_DIE 200

#define RED_GOOMBA_POINT_OFFSET_Y 16

#define RED_GOOMBA_FIRST_MID_JUMP_COUNT	4
#define RED_GOOMBA_SECOND_MID_JUMP_COUNT	8
#define RED_GOOMBA_MAXIMUM_JUMP_COUNT	12

class CRedGoomba : public CGameObject
{
protected:
	float ax;
	float ay;
	
	int jumpCount;

	bool isHaveWing = false;

	ULONGLONG die_start;

	CFallDetector* fallDetector;
	CPoint* score;
	CWing* leftWing;
	CWing* rightWing;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return ((state != RED_GOOMBA_STATE_DIE) && (state != RED_GOOMBA_STATE_JUMP_DIE)); };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CRedGoomba(float x, float y, bool isHaveWing = false);
	virtual void SetState(int state);

	bool GetIsHaveWing() { return isHaveWing; }
	void SetIsHaveWing(bool isHaveWing) { this->isHaveWing = isHaveWing; }
};