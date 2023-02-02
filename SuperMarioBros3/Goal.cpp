#include "Goal.h"

void CGoal::Render()
{
	clearText->DrawTextString("COURSE CLEAR", 2264, 80);
	clearText->DrawTextString("YOU GOT A CARD", 2264, 90);
}

void CGoal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}