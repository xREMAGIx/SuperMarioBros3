#pragma once
#include "GameObject.h"

#define BOOMERANG_WIDTH 11
#define BOOMERANG_HEIGHT 11

#define BOOMERANG_BBOX_WIDTH 11
#define BOOMERANG_BBOX_HEIGHT 11

#define BOOMERANG_SPEED 0.05f

#define BOOMERANG_STATE_IDLE 100
#define BOOMERANG_STATE_THROWN 200
#define BOOMERANG_STATE_DESTROYED 300

#define BOOMERANG_ANI_SET_ID 335

#define BOOMERANG_ANI_IDLE 0
#define BOOMERANG_ANI_THROWN 1

class CBoomerang : public CGameObject
{
public:
	CBoomerang();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
};
