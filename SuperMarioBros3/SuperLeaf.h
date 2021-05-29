#pragma once
#include "GameObject.h"

#define SUPER_LEAF_WIDTH 16
#define SUPER_LEAF_HEIGHT 16

#define SUPER_LEAF_BBOX_WIDTH 16
#define SUPER_LEAF_BBOX_HEIGHT 16


#define SUPER_LEAF_SPEED_X 0.05f
#define SUPER_LEAF_SPEED_Y 0.03f

class CSuperLeaf : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

private:
	float vx;
	float vy;
public:
	CSuperLeaf(float x, float y);	
	virtual void SetState(int state);
};
