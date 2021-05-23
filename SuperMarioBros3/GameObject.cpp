#include "GameObject.h"

vector<LPANIMATION> CGameObject::animations;

CGameObject::CGameObject(float x, float y)
{
	this->x = x;
	this->y = y;
}

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CGameObject::Render()
{
}

void CGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}



CGameObject::~CGameObject()
{
}
