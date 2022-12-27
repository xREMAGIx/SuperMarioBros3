#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Sprite.h"
#include "Sprites.h"

#define SUPER_LEAF_WIDTH 16
#define SUPER_LEAF_HEIGHT 16

#define SUPER_LEAF_BBOX_WIDTH 16
#define SUPER_LEAF_BBOX_HEIGHT 16

#define SUPER_LEAF_STATE_IDLE 1
#define SUPER_LEAF_STATE_FLY_UP	2
#define SUPER_LEAF_STATE_FALL 3
#define SUPER_LEAF_STATE_CHANGE_DIRECTION 4
#define SUPER_LEAF_STATE_EARNED 5

#define SUPER_LEAF_SPEED_X 0.03f
#define SUPER_LEAF_GRAVITY 0.0002f
#define SUPER_LEAF_BACK_Y	0.01f
#define SUPER_LEAF_FLY_UP_Y	0.2f
#define SUPER_LEAF_FALL_Y	0.0005f

#define SUPER_LEFT_FLY_TIME	500
#define SUPER_LEFT_CHANGE_DIRECTION_TIME	1500

class CSuperLeaf : public CGameObject
{
	float ay;				// acceleration on y 

	ULONGLONG start_fly;
	ULONGLONG start_change_direction;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CSuperLeaf(float x, float y);
	virtual void SetState(int state);

	void StartFly() { start_fly = GetTickCount64(); }
	void StartChangeDirection() { start_change_direction = GetTickCount64(); }
};
