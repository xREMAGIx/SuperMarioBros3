#pragma once
#include "GameObject.h"

#define GREEN_KOOPA_WIDTH 16
#define GREEN_KOOPA_HEIGHT 16
#define GREEN_KOOPA_SPEED 0.05f
#define GREEN_KOOPA_SCROLL 0.1f
#define GREEN_KOOPA_GRAVITY	0.001f


#define GREEN_KOOPA_BBOX_WIDTH 16
#define GREEN_KOOPA_BBOX_HEIGHT 27
#define GREEN_KOOPA_BBOX_SHELL_HEIGHT 16

#define GREEN_KOOPA_STATE_WALKING 100
#define GREEN_KOOPA_STATE_RESPAWN 200
#define GREEN_KOOPA_STATE_SHELL 300
#define GREEN_KOOPA_STATE_SHELL_SCROLL 400
#define GREEN_KOOPA_STATE_SHELL_HOLD 500


#define GREEN_KOOPA_ANI_WALKING 0
#define GREEN_KOOPA_ANI_SHELL 1
#define GREEN_KOOPA_ANI_SHELL_SCROLL 2
#define GREEN_KOOPA_ANI_RESPAWN 3

class CGreenKoopa : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CGreenKoopa();
	virtual void SetState(int state);
};
