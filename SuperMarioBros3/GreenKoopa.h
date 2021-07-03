#pragma once
#include "GameObject.h"

#define GREEN_KOOPA_WIDTH 16
#define GREEN_KOOPA_HEIGHT 16
#define GREEN_KOOPA_SPEED 0.02f
#define GREEN_KOOPA_GRAVITY	0.001f


#define GREEN_KOOPA_BBOX_WIDTH 16
#define GREEN_KOOPA_BBOX_HEIGHT 26
#define GREEN_KOOPA_BBOX_HEIGHT_DIE 9

#define GREEN_KOOPA_STATE_WALKING 100
#define GREEN_KOOPA_STATE_DIE 200

#define GREEN_KOOPA_ANI_WALKING 0
#define GREEN_KOOPA_ANI_DIE 1

class CGreenKoopa : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CGreenKoopa();
	virtual void SetState(int state);
};
