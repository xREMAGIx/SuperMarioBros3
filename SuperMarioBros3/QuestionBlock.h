#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define	QUESTION_BLOCK_WIDTH 16
#define QUESTION_BLOCK_BBOX_WIDTH 16
#define QUESTION_BLOCK_BBOX_HEIGHT 16

#define QUESTION_BLOCK_STATE_IDLE 100
#define QUESTION_BLOCK_STATE_OPENED 200

class CQuestionBlock : public CGameObject {
protected:
	virtual void Render();

public:
	CQuestionBlock(float x, float y) : CGameObject(x, y) {}
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);
};