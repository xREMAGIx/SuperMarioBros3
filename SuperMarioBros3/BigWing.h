#pragma once
#include "GameObject.h"


class CBigWing : public CGameObject
{
public:
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	CBigWing(float x, float y);
};

typedef CBigWing* LPBIGWING;
