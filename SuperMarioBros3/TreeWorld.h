#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define TREE_WORLD_BBOX_WIDTH	16
#define TREE_WORLD_BBOX_HEIGHT	16

class CTreeWorld : public CGameObject
{

public:
	CTreeWorld(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

};
