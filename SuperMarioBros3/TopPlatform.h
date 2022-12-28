#pragma once

#include "GameObject.h"
#include "Mario.h"

class CTopPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;

	BOOLEAN isObjectOnTop;

public:
	CTopPlatform(float x, float y,
		float cell_width, float cell_height, int length) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		isObjectOnTop = true;
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsTopPlatform() {
		return 1;
	}
	
	void SetIsObjectOnTop(BOOLEAN isOnTop) {
		this->isObjectOnTop = isOnTop;
	}

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

typedef CTopPlatform* LPTOPPLATFORM;