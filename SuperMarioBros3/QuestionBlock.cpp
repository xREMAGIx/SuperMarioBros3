#include "Game.h"
#include "QuestionBlock.h"

void CQuestionBlock::Render()
{
	int ani;
	if (state == QUESTION_BLOCK_STATE_OPENED)
	{
		ani = QUESTION_BLOCK_STATE_OPENED;
	}
	else {
		ani = QUESTION_BLOCK_ANI_IDLE;
	}
	animations[ani]->Render(x, y, nx);
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QUESTION_BLOCK_BBOX_WIDTH;
	b = y + QUESTION_BLOCK_BBOX_HEIGHT;
}