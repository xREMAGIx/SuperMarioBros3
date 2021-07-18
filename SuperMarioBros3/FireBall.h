#pragma once
#include "GameObject.h"

#define FIREBALL_WIDTH 11
#define FIREBALL_HEIGHT 11

#define FIREBALL_BBOX_WIDTH 11
#define FIREBALL_BBOX_HEIGHT 11

#define FIREBALL_SPEED 0.05f

#define FIREBALL_STATE_THROWN 100
#define FIREBALL_STATE_DESTROYED 200

#define FIREBALL_ANI_SET_ID 333

#define FIREBALL_ANI_THROWN 0


class CFireball : public CGameObject
{

public:
	CFireball();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
};
