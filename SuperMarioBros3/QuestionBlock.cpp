#include "QuestionBlock.h"

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
	case OBJECT_TYPE_SUPER_LEAF:
		item = new CSuperLeaf(x, y);
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
	CGameObject::Update(dt, coObjects);
}

void CQuestionBlock::SetState(int state)
{
	if (state == QUESTION_BLOCK_STATE_OPENED) {
		vector<LPGAMEOBJECT>& objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetGameObjects();
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		if (dynamic_cast<CCoin*>(item)) {
			item->SetState(COIN_STATE_JUMP);
		}
		else if (dynamic_cast<CSuperLeaf*>(item)) {
			if (mario->GetLevel() != MARIO_LEVEL_BIG) {
				item = new CMushroom(x, y - 2);
				item->SetState(MUSHSHROOM_STATE_SHOWING);
			}
			else {
				item->SetState(SUPER_LEAF_STATE_FLY_UP);
			}
		}
		else if (dynamic_cast<CMushroom*>(item)) {
			item->SetState(MUSHSHROOM_STATE_SHOWING);
		}
		else if (dynamic_cast<CGreenMushroom*>(item)) {
			item->SetState(GREEN_MUSHSHROOM_STATE_SHOWING);
		}
		item->SetRenderOrder(0);
		objects.push_back(item);
	}
	CGameObject::SetState(state);
}
