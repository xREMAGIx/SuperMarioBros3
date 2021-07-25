#pragma once
#include "GameObject.h"

//State
#define PLAYER_FONT_STATE_ONE  0
#define PLAYER_FONT_STATE_TWO  1

//State
#define PLAYER_FONT_SPRITE_ONE  50005
#define PLAYER_FONT_SPRITE_TWO  50006

class CPlayerFont : public CGameObject
{
	int sceneId;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);

	int GetSceneId() { return sceneId; }
	void SetSceneId(int sceneId) { this->sceneId = sceneId; }
};
