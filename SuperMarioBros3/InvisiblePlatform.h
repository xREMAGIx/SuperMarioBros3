#pragma once
#include "GameObject.h"

#define INVISIBLE_PLATFORM_BBOX_HEIGHT 32

class CInvisiblePlatform : public CGameObject
{
	int width;
	int height;
public:
	CInvisiblePlatform(int width);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

};