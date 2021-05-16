#pragma once
#include "GameObject.h"

#define QUESTION_BLOCK_WIDTH 16
#define QUESTION_BLOCK_HEIGHT 16

class CQuestionBlock : public CGameObject
{
public:
	CQuestionBlock(float x, float y);
	void Update(DWORD dt);
	void Render();
};
