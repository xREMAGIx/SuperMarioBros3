#pragma once
#include "GameObject.h"

#define INVISIBLE_BLOCK_BBOX_WIDTH 16

class CInvisibleWall : public CGameObject
{
	int width;
	int height;
public:
	CInvisibleWall(int width);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};