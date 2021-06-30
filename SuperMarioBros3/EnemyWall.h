#pragma once
#include "GameObject.h"

#define ENEMY_WALL_BBOX_WIDTH 16

class CEnemyWall : public CGameObject
{
	int height;
public:
	CEnemyWall(int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};