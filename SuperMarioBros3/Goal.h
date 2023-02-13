#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "Font.h"
#include "GameBoard.h"

#define GOAL_STATE_IDLE	1
#define GOAL_STATE_CLEAR 2

#define ITEM_BOX_MUSHROOM 1
#define ITEM_BOX_FLOWER	2
#define ITEM_BOX_STAR 3

#define GOAL_ITEM_BOX_BBOX_WIDTH 20
#define GOAL_ITEM_BOX_BBOX_HEIGHT	20

#define GOAL_TEXT_LINE_OFFSET	12
#define GOAL_TEXT_FIRST_LINE	"COURSE CLEAR"
#define GOAL_TEXT_SECOND_LINE	"YOU GOT A CARD"
#define GOAL_TEXT_SECOND_LINE_CHAR	14
#define GOAL_TEXT_SECOND_LINE_OFFSET	12

#define GOAL_ITEM_FLY_SPEED	0.1f

#define GOAL_ITEM_SHUFFLE_TIME	500
#define GOAL_TEXT_FIRST_LINE_TIME	1000
#define GOAL_TEXT_SECOND_LINE_TIME	1000
#define GOAL_FINISH_TIME	3000

class CGoal : public CGameObject {
	CFont* clearText;

	float initClearX = -1;
	float initClearY = -1;

	int itemBox;
	float itemEarnedX = -1;
	float itemEarnedY = -1;
	float itemEarnedVy = -1;

	bool isShowFirstText = false;
	bool isShowSecondText = false;

	ULONGLONG itemSwitchStart;
	ULONGLONG textFirstLineStart;
	ULONGLONG textSecondLineStart;
	ULONGLONG finishStart;

	virtual int IsCollidable() {
		if (state == GOAL_STATE_IDLE) {
			return 1;
		}
		return 0;
	};
	virtual int IsBlocking() {
		if (state == GOAL_STATE_IDLE) {
			return 1;
		}
		return 0;
	}

public:
	CGoal(float x, float y);

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

	void StartSwitchItem() {
		itemSwitchStart = GetTickCount64();
	}

	void StartTextFirstLine() {
		textFirstLineStart = GetTickCount64();
	}

	void StartTextSecondLine() {
		isShowFirstText = true;
		textSecondLineStart = GetTickCount64();
	}

	void StartFinish() {
		isShowSecondText = true;
		finishStart = GetTickCount64();
	}

	void SwitchItem();
};