#pragma once
#include "GameObject.h"

#define POINT_WIDTH 16
#define POINT_HEIGHT 16

#define POINT_STATE_SHOW 100
#define POINT_STATE_HIDE 200

#define POINT_JUMP_SPEED 0.1f

#define POINT_ANI_SHOW 0
#define POINT_ANI_HIDE -1

#define TIME_POINT_JUMP 500

#define POINT_ID_100	1
#define POINT_ID_200	2
#define POINT_ID_400	3
#define POINT_ID_800	4
#define POINT_ID_1000	5
#define POINT_ID_2000	6
#define POINT_ID_4000	7
#define POINT_ID_8000	8
#define POINT_ID_LEVELUP	0

#define POINT_SPRITE_ID_100	40037
#define POINT_SPRITE_ID_200	40038
#define POINT_SPRITE_ID_400	40039
#define POINT_SPRITE_ID_800	40040
#define POINT_SPRITE_ID_1000	40041
#define POINT_SPRITE_ID_2000	40042
#define POINT_SPRITE_ID_4000	40043
#define POINT_SPRITE_ID_8000	40044
#define POINT_SPRITE_ID_LEVELUP	40045

class CPoint : public CGameObject
{
	DWORD dt_jump;
	int point_id;

public:
	CPoint();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	void SetPointId(int point_id) { this->point_id = point_id; };

	void StartJump() { dt_jump = GetTickCount(); }
};
