#pragma once

#include "GameObject.h"
#include "Sprite.h"
#include "Sprites.h"
#include "AssetIDs.h"

#define ITEM_BOX_WIDTH 25

#define ITEM_BOX_STATE_EMPTY 100
#define ITEM_BOX_STATE_MUSHROOM 200
#define ITEM_BOX_STATE_FLOWER 300
#define ITEM_BOX_STATE_STAR 400

class CItemBox : public CGameObject
{

public:
	CItemBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void SetState(int state);
};
