#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define GREEN_MUSHROOM_GRAVITY 0.002f
#define GREEN_MUSHROOM_WALKING_SPEED 0.05f

#define	GREEN_MUSHROOM_WIDTH 16
#define GREEN_MUSHROOM_BBOX_WIDTH 16
#define GREEN_MUSHROOM_BBOX_HEIGHT 16

class CGreenMushroom : public CGameObject {
protected:
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CGreenMushroom(float x, float y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};