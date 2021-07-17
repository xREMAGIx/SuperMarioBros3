#pragma once
#include "GameObject.h"
#include "Point.h"
#include "Boomerang.h"

#define BOOMERANG_BROTHER_WIDTH 16
#define BOOMERANG_BROTHER_HEIGHT 24
#define BOOMERANG_BROTHER_SPEED 0.05f
#define BOOMERANG_BROTHER_GRAVITY	0.001f
#define BOOMERANG_BROTHER_JUMP_SPEED 0.1f

#define TIME_BOOMERANG_BROTHER_THROW 500

#define BOOMERANG_BROTHER_BBOX_WIDTH 16
#define BOOMERANG_BROTHER_BBOX_HEIGHT 24

#define BOOMERANG_BROTHER_STATE_WALKING 100
#define BOOMERANG_BROTHER_STATE_DIE 200
#define BOOMERANG_BROTHER_STATE_THROW 300

#define BOOMERANG_BROTHER_ANI_WALKING 0

class CBoomerangBrother : public CGameObject
{
	DWORD dt_throw;
	CPoint* score;
	CBoomerang* boomerang;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CBoomerangBrother();
	virtual void SetState(int state);
	void StartThrow() { dt_throw = GetTickCount(); }
	void StartWaiting() { dt_throw = 0; }
};
