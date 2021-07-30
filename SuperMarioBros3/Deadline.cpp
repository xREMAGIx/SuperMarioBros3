#include "Deadline.h"


CDeadline::CDeadline(int width)
{
	this->width = width;
}

void CDeadline::Render()
{
}

void CDeadline::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + DEADLINE_BBOX_HEIGHT;
}
