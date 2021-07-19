#pragma once
#include "GameObject.h"
#include "BigWing.h"

#define PARA_KOOPA_WIDTH 16
#define PARA_KOOPA_HEIGHT 16
#define PARA_KOOPA_SPEED 0.02f
#define PARA_KOOPA_GRAVITY	0.001f
#define PARA_KOOPA_JUMP_SPEED 0.3f
#define PARA_KOOPA_SCROLL 0.1f

#define PARA_KOOPA_BBOX_WIDTH 16
#define PARA_KOOPA_BBOX_HEIGHT 26
#define PARA_KOOPA_BBOX_SHELL_HEIGHT 16

#define PARA_KOOPA_STATE_WALKING 100
#define PARA_KOOPA_STATE_RESPAWN 200
#define PARA_KOOPA_STATE_SHELL 300
#define PARA_KOOPA_STATE_SHELL_SCROLL 400
#define PARA_KOOPA_STATE_SHELL_HOLD 500
#define PARA_KOOPA_STATE_JUMP 600
#define PARA_KOOPA_STATE_FLY 700

#define PARA_KOOPA_ANI_WALKING 0
#define PARA_KOOPA_ANI_SHELL 1
#define PARA_KOOPA_ANI_SHELL_SCROLL 2
#define PARA_KOOPA_ANI_RESPAWN 3

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
