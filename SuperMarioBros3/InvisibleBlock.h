#pragma once
#include "GameObject.h"

#define INVISIBLE_BLOCK_BBOX_HEIGHT 2

class CInvisibleBlock : public CGameObject
{
	int width;
	int height;
public:
	CInvisibleBlock(int width);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};