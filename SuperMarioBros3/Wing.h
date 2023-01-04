#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define WING_WIDTH 8
#define WING_HEIGHT 14

#define WING_BBOX_WIDTH 8
#define WING_BBOX_HEIGHT 14

#define WING_STATE_IDLE	1
#define WING_STATE_FLAP 2
#define WING_STATE_DESTROY 3

class CWing : public CGameObject
{
protected:
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; };

public:
	CWing(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};