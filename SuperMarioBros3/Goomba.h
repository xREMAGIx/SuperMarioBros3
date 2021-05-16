#pragma once
#include "GameObject.h"

#define GOOMBA_WIDTH 16
#define GOOMBA_HEIGHT 16
#define GOOMBA_SPEED 0.05f

class CGoomba : public CGameObject
{
private:
	float vx;
public:
	CGoomba(float x, float y);
	void Update(DWORD dt);
	void Render();
};
