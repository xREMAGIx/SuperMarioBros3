#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "FallDetector.h"
#include "Point.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_JUMP_DIE_SPEED	0.5f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500
#define GOOMBA_JUMP_DIE_TIMEOUT 1000

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_JUMP_DIE 300

#define GOOMBA_POINT_DIE 100
#define GOOMBA_POINT_JUMP_DIE 200

#define	GOOMBA_POINT_OFFSET_Y	16

class CGoomba : public CGameObject
{
protected: 
	float ax;				
	float ay; 

	ULONGLONG die_start;

	CFallDetector* fallDetector;
	CPoint* score;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return ((state != GOOMBA_STATE_DIE) && (state != GOOMBA_POINT_JUMP_DIE)); };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);
};