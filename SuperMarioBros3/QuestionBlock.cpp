#include "Game.h"
#include "QuestionBlock.h"

CQuestionBlock::CQuestionBlock(int _id_item, int ani_set_id) {
	switch (_id_item)
	{
	case OBJECT_TYPE_MUSHROOM:
		item = new CMushroom();
		break;
	default:
		item = new CCoin();
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		item->SetAnimationSet(ani_set);
		break;
	}
	//General setup
	item->SetId(_id_item);
}

void CQuestionBlock::Render()
{
	int ani;
	if (state == QUESTION_BLOCK_STATE_OPENED)
	{
		ani = QUESTION_BLOCK_ANI_OPENED;
		item->Render();
	}
	else {
		ani = QUESTION_BLOCK_ANI_IDLE;
	}
	animation_set->at(ani)->Render(x, y, -1, 255);
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (state == QUESTION_BLOCK_STATE_OPENED) {
		item->Update(dt, coObjects);
	}
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QUESTION_BLOCK_BBOX_WIDTH;
	b = y + QUESTION_BLOCK_BBOX_HEIGHT;
}

void CQuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BLOCK_STATE_OPENED:
		item->SetPosition(x, y - 18);
		switch (item->GetId())
		{
		case OBJECT_TYPE_MUSHROOM:
			dynamic_cast<CMushroom*>(item)->SetState(MUSHSHROOM_STATE_WALKING);
			break;
		default:
			dynamic_cast<CCoin*>(item)->SetState(COIN_STATE_JUMP);
			break;
		}
		break;
	case QUESTION_BLOCK_ANI_IDLE:
		break;
	}
}
