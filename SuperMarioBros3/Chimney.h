#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "VenusFireTrap.h"
#include "GreenVenusFireTrap.h"
#include "GreenPiranhaPlant.h"

#define CHIMNEY_STATE_IDLE 100
#define CHIMNEY_STATE_CANCEL 200

#define TIME_CHIMNEY_RESET 500

#define CHIMNEY_CELL_WIDTH	16
#define CHIMNEY_CELL_HEIGHT	16

#define CHIMNEY_VENUS_FIRE_TRAP_OFFSET_X	8
#define CHIMNEY_VENUS_FIRE_TRAP_OFFSET_Y	6

class CChimney : public CGameObject
{
protected:
	int length;				// Unit: cell 
	int height;				// Unit: cell

	int cellWidth = CHIMNEY_CELL_WIDTH;
	int cellHeight = CHIMNEY_CELL_HEIGHT;

	ULONGLONG dt_reset;
	int scene_id = -1;
	int direction = 0;
	CGameObject* object;

public:
	CChimney(float x, float y, int length, int height, int _id_item, vector<LPGAMEOBJECT>* objects);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; };

	int GetSceneId() { return scene_id; }
	void SetSceneId(int scene_id) { this->scene_id = scene_id; }

	int GetDirection() { return direction; }
	void SetDirection(int direction) { this->direction = direction; }


	void SetChimneyObjectPos(float x, float y) {
		object->SetPosition(x, y);
	}

	void StartReset() {
		dt_reset = GetTickCount64();
	}

	void StopReset() {
		dt_reset = 0;
	}

	CGameObject* GetChimneyObject() {
		return this->object;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	void SetState(int state);
};

typedef CChimney* LPCHIMNEY;