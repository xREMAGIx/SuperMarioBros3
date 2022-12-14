#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define MARIO_WORLD_BBOX_WIDTH	14
#define MARIO_WORLD_BBOX_HEIGHT	16

#define MARIO_WORLD_STATE_IDLE		0
#define MARIO_WORLD_STATE_MOVING		1

#define MARIO_WORLD_WALKING_SPEED		0.1f


class CMarioWorld : public CGameObject
{
	int ny;
	float moving_x;
	float moving_y;

public:
	CMarioWorld() : CGameObject()
	{
		moving_x = 0;
		moving_y = 0;
		ny = 0;
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	void SetState(int state);
	void SetMovingPoint(float x, float y) { moving_x = x; moving_y = y; };

	void MovingPosition(float x, float y);
};
