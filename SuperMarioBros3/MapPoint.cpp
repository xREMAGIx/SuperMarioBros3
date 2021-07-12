#include "MapPoint.h"


CMapPoint::CMapPoint(int id, int dTop, int dRight, int dBottom, int dLeft, int scene_id)
{
	this->id = id;
	this->dTop = dTop;
	this->dRight = dRight;
	this->dBottom = dBottom;
	this->dLeft = dLeft;
	this->scene_id = scene_id;
}


void CMapPoint::Render()
{
}

void CMapPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}