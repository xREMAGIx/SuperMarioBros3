#pragma once

#include "GameObject.h"
#include "AssetIDs.h"

#define MAP_POINT_BBOX_WIDTH	16
#define MAP_POINT_BBOX_HEIGHT	16

class CMapPoint : public CGameObject
{
	int id;
	int dTop;
	int dRight;
	int dBottom;
	int dLeft;
	int scene_id;

public:
	CMapPoint(int id, int dTop, int dRight, int dBottom, int dLeft, int scene_id);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

	int GetId() { return id; }
	int GetDTop() { return dTop; }
	int GetDRight() { return dRight; }
	int GetDBottom() { return dBottom; }
	int GetDLeft() { return dLeft; }
	int GetSceneId() { return scene_id; }
};
