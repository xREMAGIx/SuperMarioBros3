#pragma once

#include "GameObject.h"

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

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
	void SetMovingPoint(float x, float y) { moving_x = x; moving_y = y; };

	void MovingPosition(float x, float y);
};
