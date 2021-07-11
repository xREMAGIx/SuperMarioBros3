#pragma once
#include "GameObject.h"

#define COIN_WIDTH 16
#define COIN_HEIGHT 16

#define COIN_BBOX_WIDTH 16
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_IDLE 100
#define COIN_STATE_EARNED 200
#define COIN_STATE_JUMP 300

#define COIN_JUMP_SPEED 0.1f

#define COIN_ANI_IDLE 0
#define COIN_ANI_EARNED -1

#define TIME_COIN_JUMP 500


class CCoin : public CGameObject
{
	DWORD dt_jump;

public:
	CCoin();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);

	void StartJump() { dt_jump = GetTickCount(); }
};
