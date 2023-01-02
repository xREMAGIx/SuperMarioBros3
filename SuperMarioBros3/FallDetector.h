#pragma once

#include "GameObject.h"

#define FALL_DETECTOR_GRAVITY 0.001f

#define FALL_DETECTOR_STATE_IDLE 1
#define FALL_DETECTOR_STATE_DETECT 2
#define FALL_DETECTOR_STATE_FALL	3
#define FALL_DETECTOR_STATE_IGNORE	4

#define FALL_DETECTOR_FALL_TIMEOUT 200
#define FALL_DETECTOR_FALL_IGNORE 1000

class CFallDetector : public CGameObject
{
protected:
	float ay;

	float cellWidth;
	float cellHeight;

	ULONGLONG fall_start;
	ULONGLONG ignore_start;

public:
	CFallDetector(float x, float y,
		float cell_width, float cell_height) :CGameObject(x, y)
	{
		ay = FALL_DETECTOR_GRAVITY;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		fall_start = -1;
		ignore_start = -1;
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	virtual void SetState(int state);

	void StartFall() { fall_start = GetTickCount64(); }
	void StartIgnore() { ignore_start = GetTickCount64(); }
};
