#pragma once
#include "GameObject.h"
#include "InvisibleBlock.h"
#include "QuestionBlock.h"
#include "Chimney.h"
#include "Mario.h"


#define GREEN_MUSHSHROOM_SPEED 0.05f
#define GREEN_MUSHSHROOM_SHOW_SPEED 0.01f
#define GREEN_MUSHSHROOM_GRAVITY	0.001f

//State
#define GREEN_MUSHSHROOM_STATE_WALKING 1
#define GREEN_MUSHSHROOM_STATE_EARNED 2
#define GREEN_MUSHSHROOM_STATE_SHOWING 3
//Collision
#define GREEN_MUSHSHROOM_BBOX_WIDTH  16
#define GREEN_MUSHSHROOM_BBOX_HEIGHT 16

class CGreenMushroom : public CGameObject
{
	CPoint* score;
public:
	CGreenMushroom();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};