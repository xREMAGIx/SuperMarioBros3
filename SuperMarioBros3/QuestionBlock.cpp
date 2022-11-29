#include "QuestionBlock.h"

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

void CQuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
}
