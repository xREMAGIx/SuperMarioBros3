#pragma once

#include "GameObject.h"
#include "Sprite.h"
#include "Sprites.h"
#include "AssetIDs.h"

#define POINT_FLOAT	0.02f

#define POINT_TYPE_100	1
#define POINT_TYPE_200	2
#define POINT_TYPE_400	3
#define POINT_TYPE_800	4
#define POINT_TYPE_1000	5
#define POINT_TYPE_2000	6
#define POINT_TYPE_4000	7
#define POINT_TYPE_8000	8
#define POINT_TYPE_UP	0

#define POINT_SHOW_TIME 1000

#define POINT_STATE_IDLE 100
#define POINT_STATE_SHOW 200

class CPoint : public CGameObject {
protected:
	int type;
	ULONGLONG show_start;

public:
	CPoint(float x, float y) : CGameObject(x, y) {
		vy = 0;
		state = POINT_STATE_IDLE;
		type = POINT_TYPE_100;
		show_start = -1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetType(int type) {
		this->type = type;
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int IsBlocking() { return 0; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
};