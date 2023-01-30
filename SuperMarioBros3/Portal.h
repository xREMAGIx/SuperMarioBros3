#pragma once

#include "GameObject.h"

#define PORTAL_DIRECTION_DOWN	1
#define PORTAL_DIRECTION_UP	2

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float next_pos_x;
	float next_pos_y;

	float width;
	float height;

	int portal_direction;

	int IsBlocking() { return 0; }

public:
	CPortal(float x,
		float y,
		float width,
		float height,
		int scene_id,
		float next_pos_x,
		float next_pos_y,
		int direction = PORTAL_DIRECTION_DOWN
		);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void RenderBoundingBox(void);
	
	int GetSceneId() { return scene_id;  }

	int GetPortalDirection() { return portal_direction; }

	void GetNextPos(float& x, float& y) { x = next_pos_x; y = next_pos_y; }
	void SetNextPos(float x, float y) {
		this->next_pos_x = x;
		this->next_pos_y = y;
	}
};