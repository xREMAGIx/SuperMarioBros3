#pragma once
#include "GameObject.h"

#define SMALLWING_ANI_SET_ID 316

#define SMALLWING_STATE_IDLE 0
#define SMALLWING_STATE_FLYING 1

#define SMALLWING_ANI_IDLE 0
#define SMALLWING_ANI_FLYING 1

class CSmallWing : public CGameObject
{
public:
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	CSmallWing(float x, float y);
};

typedef CSmallWing* LPSMALLWING;
