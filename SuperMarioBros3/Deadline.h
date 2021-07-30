#pragma once
#include "GameObject.h"

#define DEADLINE_BBOX_HEIGHT 2

class CDeadline : public CGameObject
{
	int width;
public:
	CDeadline(int width);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};