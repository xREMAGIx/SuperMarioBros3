#pragma once
#include "GameObject.h"
#include "VenusFireTrap.h"

#define CHIMNEY_STATE_IDLE 100
#define CHIMNEY_STATE_CANCEL 200

#define TIME_CHIMNEY_RESET 500

class CChimney : public CGameObject
{
	int width;
	int height;
	DWORD dt_reset;
	int scene_id = -1;
	int direction = 0;
	CGameObject* object;

public:
	CChimney(int width, int height, int _id_item, int ani_set_id, float x, float y, vector<LPGAMEOBJECT>* objects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);

	int GetSceneId() { return scene_id; }
	void SetSceneId(int scene_id) { this->scene_id = scene_id; }

	int GetDirection() { return direction; }
	void SetDirection(int direction) { this->direction = direction; }


	void SetChimneyObjectPos(float x, float y) {
		object->SetPosition(x, y);
	}

	void StartReset() {
		dt_reset = GetTickCount();
	}

	void StopReset() {
		dt_reset = 0;
	}

	CGameObject* GetChimneyObject() {
		return this->object;
	}
};