#pragma once
#include "GameObject.h"

#define PARA_GOOMBA_WIDTH 16
#define PARA_GOOMBA_HEIGHT 16
#define PARA_GOOMBA_SPEED 0.05f
#define PARA_GOOMBA_GRAVITY	0.001f


#define PARA_GOOMBA_BBOX_WIDTH 16
#define PARA_GOOMBA_BBOX_HEIGHT 15
#define PARA_GOOMBA_BBOX_HEIGHT_DIE 9

#define PARA_GOOMBA_STATE_WALKING 100
#define PARA_GOOMBA_STATE_DIE 200

#define PARA_GOOMBA_ANI_WALKING 0
#define PARA_GOOMBA_ANI_DIE 1

class CParaGoomba : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CParaGoomba();
	virtual void SetState(int state);
};
