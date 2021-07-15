#pragma once
#include "GameObject.h"

#define MUSIC_BOX_WIDTH 16
#define MUSIC_BOX_HEIGHT 16

#define MUSIC_BOX_BBOX_WIDTH 16
#define MUSIC_BOX_BBOX_HEIGHT 16

#define MUSIC_BOX_STATE_IDLE 100
#define MUSIC_BOX_STATE_JUMP 200
#define MUSIC_BOX_STATE_HIDDEN 300

#define MUSIC_BOX_ANI_IDLE 0
#define MUSIC_BOX_ANI_JUMP 1
#define MUSIC_BOX_ANI_SPECIAL 2

class CMusicBox : public CGameObject
{
public:
	CMusicBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
