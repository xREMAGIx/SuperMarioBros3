#pragma once
#include "GameObject.h"

#define SUPER_LEAF_WIDTH 16
#define SUPER_LEAF_HEIGHT 16
#define SUPER_LEAF_SPEED_X 0.05f
#define SUPER_LEAF_SPEED_Y 0.03f

class CSuperLeaf : public CGameObject
{
private:
	float vx;
	float vy;
public:
	CSuperLeaf(float x, float y);
	void Update(DWORD dt);
	void Render();
};
