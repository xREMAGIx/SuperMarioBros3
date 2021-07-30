#pragma once
#include "GameObject.h"
#include "helpers.h"
#include "Mushroom.h"
#include "GreenMushroom.h"
#include "Coin.h"

//State
#define QUESTION_BLOCK_STATE_IDLE  0
#define QUESTION_BLOCK_STATE_OPENED  1
//Animate
#define QUESTION_BLOCK_ANI_IDLE		0
#define QUESTION_BLOCK_ANI_OPENED		1
//Collision
#define QUESTION_BLOCK_BBOX_WIDTH	16
#define QUESTION_BLOCK_BBOX_HEIGHT	16

class CQuestionBlock : public CGameObject
{
	CGameObject* item;

public:
	CQuestionBlock(int _id_item = 6, int ani_set_id = 208);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	CGameObject* GetItem() {
		return this->item;
	}
};
