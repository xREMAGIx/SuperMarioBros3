#include "QuestionBlock.h"
#include "Coin.h"
#include "Mushroom.h"
#include "GreenMushroom.h"

CQuestionBlock::CQuestionBlock(float x, float y, int item_id) :CGameObject(x, y)
{
	switch (item_id)
	{
	case OBJECT_TYPE_MUSHROOM:
		item = new CMushroom(x, y);
		break;
	case OBJECT_TYPE_GREEN_MUSHROOM:
		item = new CGreenMushroom(x, y);
		break;
	default:
		item = new CCoin(x, y);
		break;
	}

	state = QUESTION_BLOCK_STATE_IDLE;
}

void CQuestionBlock::Render()
{
	int aniId = ID_ANI_QUESTION_BLOCK_IDLE;
	if (state == QUESTION_BLOCK_STATE_OPENED)
	{
		aniId = ID_ANI_QUESTION_BLOCK_OPENED;
		if (!item->IsDeleted()) {
			item->Render();
		}
	}

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BLOCK_BBOX_WIDTH / 2;
	t = y - QUESTION_BLOCK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BLOCK_BBOX_WIDTH;
	b = t + QUESTION_BLOCK_BBOX_HEIGHT;
}


void CQuestionBlock::OnNoCollision(DWORD dt)
{
};

void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == QUESTION_BLOCK_STATE_OPENED) {
		item->Update(dt, coObjects);
	}
	if (!item->IsDeleted()) {
		CGameObject::Update(dt, coObjects);
	}
}

void CQuestionBlock::SetState(int state)
{
	if (state == QUESTION_BLOCK_STATE_OPENED) {
		if (dynamic_cast<CCoin*>(item)) {
			item->SetState(COIN_STATE_JUMP);
		}
	}
	CGameObject::SetState(state);
}
