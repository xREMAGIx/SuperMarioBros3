#pragma once
#include "GameObject.h"

#define COIN_WIDTH 16
#define COIN_HEIGHT 16

#define COIN_BBOX_WIDTH 16
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_IDLE 100
#define COIN_STATE_EARNED 200

#define COIN_ANI_IDLE 0
#define COIN_ANI_EARNED 1

class CCoin : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	virtual void SetState(int state);
};
