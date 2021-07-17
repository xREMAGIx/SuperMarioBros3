#pragma once
#include "GameObject.h"
#include "InvisibleBlock.h"
#include "QuestionBlock.h"
#include "Chimney.h"
#include "Mario.h"
#include "Game.h"


#define MUSHSHROOM_SPEED 0.05f
#define MUSHSHROOM_SHOW_SPEED 0.01f
#define MUSHSHROOM_GRAVITY	0.001f

//State
#define MUSHSHROOM_STATE_WALKING 1
#define MUSHSHROOM_STATE_EARNED 2
#define MUSHSHROOM_STATE_SHOWING 3
//Collision
#define MUSHSHROOM_BBOX_WIDTH  16
#define MUSHSHROOM_BBOX_HEIGHT 16

class CMushroom : public CGameObject
{
	CPoint* score;
public:
	CMushroom();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};