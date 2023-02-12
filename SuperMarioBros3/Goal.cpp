#include "Goal.h"

CGoal::CGoal(float x, float y) :CGameObject(x, y)
{
	itemSwitchStart = -1;
	itemBox = ITEM_BOX_MUSHROOM;
	state = GOAL_STATE_IDLE;
	clearText = new CFont();
}

void CGoal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GOAL_STATE_IDLE) {
		if (GetTickCount64() - itemSwitchStart > GOAL_ITEM_SHUFFLE_TIME) {
			SwitchItem();
			StartSwitchItem();
		}
		return;
	}

	if (state == GOAL_STATE_CLEAR) {
		itemEarnedY += itemEarnedVy * dt;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects, true);
}

void CGoal::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	switch (state)
	{
	case GOAL_STATE_IDLE: {
		int itemSpriteId = -1;
		switch (itemBox)
		{
		case ITEM_BOX_MUSHROOM: 
			itemSpriteId = ID_SPRITE_GOAL_ITEM_MUSHROOM;
			break;
		case ITEM_BOX_FLOWER:
			itemSpriteId = ID_SPRITE_GOAL_ITEM_FLOWER;
			break;
		case ITEM_BOX_STAR:
			itemSpriteId = ID_SPRITE_GOAL_ITEM_STAR;
			break;
		default:
			break;
		}
		sprites->Get(itemSpriteId)->Draw(x, y);
		break;
	}
	case GOAL_STATE_CLEAR: {
		int itemAniId = -1;
		int itemBoxSprite = -1;
		switch (itemBox)
		{
		case ITEM_BOX_MUSHROOM:
			itemAniId = ID_ANI_GOAL_ITEM_MUSHROOM_EARNED;
			itemBoxSprite = ID_SPRITE_ITEM_BOX_MUSHROOM;
			break;
		case ITEM_BOX_FLOWER:
			itemAniId = ID_ANI_GOAL_ITEM_FLOWER_EARNED;
			itemBoxSprite = ID_SPRITE_ITEM_BOX_FLOWER;
			break;
		case ITEM_BOX_STAR:
			itemAniId = ID_ANI_GOAL_ITEM_STAR_EARNED;
			itemBoxSprite = ID_SPRITE_ITEM_BOX_STAR;
			break;
		default:
			break;
		}
		animations->Get(itemAniId)->Render(itemEarnedX, itemEarnedY, false);
		clearText->DrawTextString(GOAL_TEXT_FIRST_LINE, initClearX, initClearY);
		clearText->DrawTextString(GOAL_TEXT_SECOND_LINE, initClearX, initClearY + GOAL_TEXT_LINE_OFFSET);
		sprites->Get(itemBoxSprite)->Draw(initClearX + (GOAL_TEXT_SECOND_LINE_CHAR / 2) * FONT_TEXT_SPACING + GOAL_TEXT_SECOND_LINE_OFFSET, initClearY + GOAL_TEXT_LINE_OFFSET);
		break;
	}
	default:
		break;
	}
	RenderBoundingBox();
}

void CGoal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GOAL_ITEM_BOX_BBOX_WIDTH / 2;
	t = y - GOAL_ITEM_BOX_BBOX_HEIGHT / 2;
	r = l + GOAL_ITEM_BOX_BBOX_WIDTH;
	b = t + GOAL_ITEM_BOX_BBOX_HEIGHT;
}

void CGoal::SwitchItem()
{
	if (itemBox != ITEM_BOX_STAR) {
		itemBox++;
	}
	else {
		itemBox = ITEM_BOX_MUSHROOM;
	}
}

void CGoal::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GOAL_STATE_IDLE: {
		StartSwitchItem();
		break;
	}
	case GOAL_STATE_CLEAR: {
		CGame* game = CGame::GetInstance();
		float camX, camY; 
		game->GetCamPos(camX, camY);
		int screenW = game->GetScreenWidth();
		int screenH = game->GetScreenHeight();

		initClearX = camX + screenW / 2;
		initClearY = camY + screenH / 4;

		itemEarnedX = x;
		itemEarnedY = y;
		itemEarnedVy = -GOAL_ITEM_FLY_SPEED;

		break;
	}
	default:
		break;
	}
}
