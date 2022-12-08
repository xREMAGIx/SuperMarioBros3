#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

class CThirdFont : public CGameObject {
protected:

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CThirdFont(float x, float y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);
};