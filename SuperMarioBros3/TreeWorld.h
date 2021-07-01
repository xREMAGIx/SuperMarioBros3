#pragma once
#include "GameObject.h"

#define TREE_WORLD_WIDTH 16
#define TREE_WORLD_HEIGHT 16

class CTreeWorld : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};
