#pragma once
#include "GameObject.h"
#include "BigWing.h"

#define PARA_KOOPA_WIDTH 16
#define PARA_KOOPA_HEIGHT 16
#define PARA_KOOPA_SPEED 0.02f
#define PARA_KOOPA_GRAVITY	0.01f
#define PARA_KOOPA_JUMP_SPEED 0.1f

#define PARA_KOOPA_BBOX_WIDTH 16
#define PARA_KOOPA_BBOX_HEIGHT 26
#define PARA_KOOPA_BBOX_HEIGHT_DIE 9

#define PARA_KOOPA_STATE_WALKING 100
#define PARA_KOOPA_STATE_DIE 200

#define PARA_KOOPA_ANI_WALKING 0
#define PARA_KOOPA_ANI_DIE 1

#define PARA_KOOPA_WING_X 8
#define PARA_KOOPA_WING_Y 1

class CParaKoopa : public CGameObject
{
	LPBIGWING wing;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CParaKoopa();
	virtual void SetState(int state);
};
