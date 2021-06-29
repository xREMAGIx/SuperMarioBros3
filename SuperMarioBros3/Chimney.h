#pragma once
#include "GameObject.h"

class CChimney : public CGameObject
{
	int width;
	int height;
public:
	CChimney(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};