#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define BRICK_BREAK_CUBE_WIDTH 8
#define BRICK_BREAK_CUBE_BBOX_WIDTH 8
#define BRICK_BREAK_CUBE_BBOX_HEIGHT 9

#define BRICK_BREAK_CUBE_HIGH_FLY_SPEED_Y	0.6f
#define BRICK_BREAK_CUBE_HIGH_FLY_SPEED_X	0.1f
#define BRICK_BREAK_CUBE_LOW_FLY_SPEED_Y	0.4f
#define BRICK_BREAK_CUBE_LOW_FLY_SPEED_X	0.1f
#define BRICK_BREAK_CUBE_GRAVITY	0.002f

#define BRICK_BREAK_CUBE_IDLE_STATE	1
#define BRICK_BREAK_CUBE_HIGH_FLY_STATE	2
#define BRICK_BREAK_CUBE_LOW_FLY_STATE	3
#define BRICK_BREAK_CUBE_TIMEOUT_STATE	4

class CBrickBreakCube : public CGameObject {

	float ay; 
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	CBrickBreakCube(float x, float y) : CGameObject(x, y) {
		ay = 0;
		state = BRICK_BREAK_CUBE_IDLE_STATE;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
};