#pragma once
#include "GameObject.h"


#define MUSHSHROOM_SPEED 0.05f

//State
#define MUSHSHROOM_STATE_WALKING 1
#define MUSHSHROOM_STATE_DIE 2
//Collision
#define MUSHSHROOM_BBOX_WIDTH  16
#define MUSHSHROOM_BBOX_HEIGHT 16

class CMushroom : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};