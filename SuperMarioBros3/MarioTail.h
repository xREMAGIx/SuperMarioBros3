#pragma once

#include "GameObject.h"

#define MARIO_TAIL_BBOX_WIDTH 12
#define MARIO_TAIL_BBOX_HEIGHT 4

#define MARIO_TAIL_MOVING_SPEED	0.05f

#define MARIO_TAIL_STATE_IDLE	1
#define MARIO_TAIL_STATE_ATTACK	2

class CMarioTail : public CGameObject {

	virtual int IsCollidable() {
		return 1;
	};
	virtual int IsBlocking() {
		return 0;
	}

public:
	CMarioTail(float x, float y) : CGameObject(x, y) {
	}
	void Render() {
		RenderBoundingBox();
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithBrick(LPCOLLISIONEVENT e);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

};