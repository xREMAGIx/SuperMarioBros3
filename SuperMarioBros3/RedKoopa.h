#pragma once
#include "GameObject.h"

#define RED_KOOPA_WIDTH 16
#define RED_KOOPA_HEIGHT 16
#define RED_KOOPA_SPEED 0.05f
#define RED_KOOPA_GRAVITY	0.001f


#define RED_KOOPA_BBOX_WIDTH 16
#define RED_KOOPA_BBOX_HEIGHT 26
#define RED_KOOPA_BBOX_HEIGHT_DIE 9

#define RED_KOOPA_STATE_WALKING 100
#define RED_KOOPA_STATE_DIE 200

#define RED_KOOPA_ANI_WALKING 0
#define RED_KOOPA_ANI_DIE 1

class CRedKoopa : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CRedKoopa();
	virtual void SetState(int state);
};
