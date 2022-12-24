#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define BLOCK_WIDTH 16
#define BLOCK_BBOX_WIDTH 16
#define BLOCK_BBOX_HEIGHT 16

class CBlock : public CGameObject {

public:
	CBlock(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};