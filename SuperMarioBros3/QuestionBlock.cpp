#include "Game.h"
#include "QuestionBlock.h"

CQuestionBlock::CQuestionBlock(float x, float y) :CGameObject(x, y)
{
};

void CQuestionBlock::Update(DWORD dt)
{
}

void CQuestionBlock::Render()
{
	LPANIMATION ani;
	ani = CAnimations::GetInstance()->Get(21001);

	ani->Render(x, y);
}