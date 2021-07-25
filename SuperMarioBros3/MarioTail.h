#pragma once
#include "GameObject.h"

#define MARIO_TAIL_BBOX_WIDTH 24
#define MARIO_TAIL_BBOX_HEIGHT 16

#define MARIO_TAIL_OFFSET_WIDTH 8
#define MARIO_TAIL_OFFSET_HEIGHT 11

class CMarioTail : public CGameObject
{

public:
	CMarioTail();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
};
