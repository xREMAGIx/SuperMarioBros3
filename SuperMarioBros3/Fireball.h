#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "PlayScene.h"

#define FIREBALL_WIDTH 11
#define FIREBALL_HEIGHT 11

#define FIREBALL_BBOX_WIDTH 11
#define FIREBALL_BBOX_HEIGHT 11

#define FIREBALL_SPEED 0.05f

#define FIREBALL_STATE_IDLE	1
#define FIREBALL_STATE_THROWN 2
#define FIREBALL_STATE_DESTROYED 3

class CFireball : public CGameObject
{
protected:

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CFireball(float x, float y);


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};