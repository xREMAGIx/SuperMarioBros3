#include "Block.h"

void CBlock::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20022);
	sprite->Draw(x, y);
	//RenderBoundingBox();
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLOCK_BBOX_WIDTH;
	b = y + BLOCK_BBOX_HEIGHT;
}