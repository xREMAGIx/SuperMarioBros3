#include "Game.h"
#include "Goomba.h"
#include "ItemBox.h"

CItemBox::CItemBox()
{
	SetState(ITEM_BOX_STATE_EMPTY);
}


void CItemBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CItemBox::Render()
{
	int spriteId;
	LPSPRITE sprite;

	switch (state)
	{
		case ITEM_BOX_STATE_MUSHROOM:
			spriteId = ITEM_BOX_SPRITE_ID_MUSHROOM;
			break;
		case ITEM_BOX_STATE_FLOWER:
			spriteId = ITEM_BOX_SPRITE_ID_FLOWER;
			break;
		case ITEM_BOX_STATE_STAR:
			spriteId = ITEM_BOX_SPRITE_ID_STAR;
			break;
		default:
			spriteId = ITEM_BOX_SPRITE_ID_EMPTY;
			break;
	}

	sprite = CSprites::GetInstance()->Get(spriteId);
	sprite->Draw(x, y);
}

void CItemBox::SetState(int state)
{
	CGameObject::SetState(state);
}
