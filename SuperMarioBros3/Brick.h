#pragma once
#include "GameObject.h"

#define BRICK_WIDTH 16
#define BRICK_HEIGHT 16

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_IDLE 100
#define BRICK_STATE_BREAK 200

#define BRICK_ANI_IDLE 0
#define BRICK_ANI_BREAK 1

class CBrick : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	virtual void SetState(int state);
};
