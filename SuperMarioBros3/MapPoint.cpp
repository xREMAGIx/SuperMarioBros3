#include "MapPoint.h"


CMapPoint::CMapPoint(int id, int dTop, int dRight, int dBottom, int dLeft)
{
	this->id = id;
	this->dTop = dTop;
	this->dRight = dRight;
	this->dBottom = dBottom;
	this->dLeft = dLeft;
}


void CMapPoint::Render()
{
}

void CMapPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}