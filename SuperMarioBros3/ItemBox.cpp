#include "ItemBox.h"

CItemBox::CItemBox()
{
	state = ITEM_BOX_STATE_EMPTY;
}


void CItemBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CItemBox::Render()
{
	int spriteId;

	switch (state)
	{
	case ITEM_BOX_STATE_MUSHROOM:
		spriteId = ID_SPRITE_ITEM_BOX_MUSHROOM;
		break;
	case ITEM_BOX_STATE_FLOWER:
		spriteId = ID_SPRITE_ITEM_BOX_FLOWER;
		break;
	case ITEM_BOX_STATE_STAR:
		spriteId = ID_SPRITE_ITEM_BOX_STAR;
		break;
	default:
		spriteId = ID_SPRITE_ITEM_BOX_EMPTY;
		break;
	}

	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(spriteId)->Draw(x, y);
}

void CItemBox::SetState(int state)
{
	CGameObject::SetState(state);
}
