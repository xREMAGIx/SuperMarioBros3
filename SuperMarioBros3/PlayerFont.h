#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define PLAYER_FONT_STATE_ONE 1
#define PLAYER_FONT_STATE_TWO 2

class CPlayerFont : public CGameObject {
protected:
	int sceneId;

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CPlayerFont(float x, float y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);
};