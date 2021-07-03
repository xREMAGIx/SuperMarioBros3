#pragma once
#include "GameObject.h"


class CSmallWing : public CGameObject
{
public:
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	CSmallWing(float x, float y);
};

typedef CSmallWing* LPSMALLWING;
