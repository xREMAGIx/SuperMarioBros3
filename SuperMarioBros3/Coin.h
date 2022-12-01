#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Point.h"

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_IDLE	1
#define COIN_STATE_EARNED	2
#define COIN_STATE_JUMP	3
#define COIN_STATE_JUMP_POINT	4

#define COIN_JUMP_SPEED	0.5f 
#define COIN_GRAVITY	0.002f

#define COIN_JUMP_TIME	400
#define COIN_POINT_JUMP_TIME	100

#define COIN_POINT 1000

class CCoin : public CGameObject {
protected: 
	CPoint* point;
	ULONGLONG jump_start;
	float ay;				// acceleration on y 

public:
	CCoin(float x, float y) : CGameObject(x, y) {
		vx = 0;
		vy = 0;
		jump_start = -1;
		point = new CPoint(x, y - 16);
		point->SetType(POINT_TYPE_1000);
		ay = 0;
	}
	int IsBlocking() { return 0; }

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
};