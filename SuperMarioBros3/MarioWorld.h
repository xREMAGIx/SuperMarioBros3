#pragma once

#include "GameObject.h"

#define MARIO_WORLD_STATE_SMALL		0

class CMarioWorld : public CGameObject
{

public:
	CMarioWorld() : CGameObject()
	{

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
};
