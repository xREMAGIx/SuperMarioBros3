#pragma once
#include "GameObject.h"

#define FIREBALL_WIDTH 11
#define FIREBALL_HEIGHT 11

#define FIREBALL_BBOX_WIDTH 11
#define FIREBALL_BBOX_HEIGHT 11

#define FIREBALL_STATE_IDLE 100


#define FIREBALL_ANI_IDLE 0

#define TIME_FIREBALL_JUMP 500


class CFireball : public CGameObject
{
public:
	CFireball();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
};
