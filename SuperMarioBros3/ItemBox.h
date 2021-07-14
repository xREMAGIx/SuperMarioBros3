#pragma once
#include "GameObject.h"

#define ITEM_BOX_WIDTH 25

#define ITEM_BOX_STATE_EMPTY 100
#define ITEM_BOX_STATE_MUSHROOM 200
#define ITEM_BOX_STATE_FLOWER 300
#define ITEM_BOX_STATE_STAR 400

#define ITEM_BOX_SPRITE_ID_EMPTY 40053
#define ITEM_BOX_SPRITE_ID_MUSHROOM 40054
#define ITEM_BOX_SPRITE_ID_FLOWER 40055
#define ITEM_BOX_SPRITE_ID_STAR 40056

class CItemBox : public CGameObject
{

public:
	CItemBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void SetState(int state);
};
