#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "PlayScene.h"

#include "Coin.h"
#include "Mushroom.h"
#include "GreenMushroom.h"
#include "SuperLeaf.h"

#define	QUESTION_BLOCK_WIDTH 16
#define QUESTION_BLOCK_BBOX_WIDTH 16
#define QUESTION_BLOCK_BBOX_HEIGHT 16

#define QUESTION_BLOCK_STATE_IDLE 100
#define QUESTION_BLOCK_STATE_OPENED 200

class CQuestionBlock : public CGameObject {
protected:
	CGameObject* item;

	virtual void Render();
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CQuestionBlock(float x, float y, int item_id);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
};