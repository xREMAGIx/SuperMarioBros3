#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "PlayScene.h"

#include "BrickBreakCube.h"
#include "PSwitch.h"

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_IDLE	1
#define BRICK_STATE_BREAK	2
#define BRICK_STATE_TURNED	3
#define BRICK_STATE_EARNED	4

#define BRICK_BREAK_TIME	5000

class CBrick : public CGameObject {
	CGameObject* item;

	CBrickBreakCube* topLeftCube;
	CBrickBreakCube* bottomLeftCube;
	CBrickBreakCube* topRightCube;
	CBrickBreakCube* bottomRightCube;

	ULONGLONG break_start;

	virtual int IsCollidable() { 
		if (state == BRICK_STATE_IDLE || state == BRICK_STATE_TURNED) {
			return 1;
		}
		return 0;
	};
	virtual int IsBlocking() {
		if (state == BRICK_STATE_IDLE) {
			return 1;
		}
		return 0;
	}

public:
	CBrick(float x, float y, int item_id = -1) : CGameObject(x, y) {
		break_start = -1;
		state = BRICK_STATE_IDLE;
		topLeftCube = new CBrickBreakCube(x, y);
		bottomLeftCube = new CBrickBreakCube(x, y);
		topRightCube = new CBrickBreakCube(x, y);
		bottomRightCube = new CBrickBreakCube(x, y);

		switch (item_id)
		{
		case OBJECT_TYPE_P_SWITCH:
			item = new CPSwitch(x, y);
			break;
		default:
			item = NULL;
			break;
		}
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);


	void StartBreak() {
		break_start = GetTickCount64();
	}
};