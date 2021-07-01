#pragma once
#include "GameObject.h"

class CMapPoint : public CGameObject
{
	int id;
	int dTop;
	int dRight;
	int dBottom;
	int dLeft;
public:
	CMapPoint(int id, int dTop, int dRight, int dBottom, int dLeft);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	int GetId() { return id; }
	int GetDTop() { return dTop; }
	int GetDRight() { return dRight; }
	int GetDBottom() { return dBottom; }
	int GetDLeft() { return dLeft; }
};