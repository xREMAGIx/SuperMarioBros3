#pragma once
#include "GameObject.h"
#include "Point.h"

#define GOOMBA_WIDTH 16
#define GOOMBA_HEIGHT 16
#define GOOMBA_SPEED 0.05f
#define GOOMBA_GRAVITY	0.001f

#define TIME_GOOMBA_DIE 1000

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

class CGoomba : public CGameObject
{
	DWORD dt_die;
	CPoint* score;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CGoomba();
	virtual void SetState(int state);
	void StartDie() { dt_die = GetTickCount(); }

};
