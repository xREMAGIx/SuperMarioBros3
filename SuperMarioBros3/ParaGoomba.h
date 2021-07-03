#pragma once
#include "GameObject.h"
#include "SmallWing.h"

#define PARA_GOOMBA_WIDTH 16
#define PARA_GOOMBA_HEIGHT 16
#define PARA_GOOMBA_SPEED 0.05f
#define PARA_GOOMBA_JUMP_SPEED 0.1f
#define PARA_GOOMBA_GRAVITY	0.001f


#define PARA_GOOMBA_BBOX_WIDTH 16
#define PARA_GOOMBA_BBOX_HEIGHT 15
#define PARA_GOOMBA_BBOX_HEIGHT_DIE 9

#define PARA_GOOMBA_STATE_WALKING 100
#define PARA_GOOMBA_STATE_DIE 200
#define PARA_GOOMBA_STATE_JUMP_SMALL 300
#define PARA_GOOMBA_STATE_JUMP_BIG 400

#define PARA_GOOMBA_ANI_WALKING 0
#define PARA_GOOMBA_ANI_DIE 1

#define PARA_GOOMBA_LEFT_WING_X 10
#define PARA_GOOMBA_LEFT_WING_Y 2
#define PARA_GOOMBA_RIGHT_WING_X -2
#define PARA_GOOMBA_RIGHT_WING_Y 2

class CParaGoomba : public CGameObject
{
	int jumpCount;

	LPSMALLWING leftWing;
	LPSMALLWING rightWing;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CParaGoomba(float x, float y);
	virtual void SetState(int state);
};
