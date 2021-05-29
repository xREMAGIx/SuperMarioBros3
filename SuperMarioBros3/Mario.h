#pragma once

#include "GameObject.h"

#define MARIO_WIDTH 14
#define MARIO_WALKING_SPEED		0.08f
#define MARIO_JUMP_SPEED_Y		0.4f
#define MARIO_GRAVITY			0.05f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300

#define MARIO_ANI_IDLE_LEFT			0
#define MARIO_ANI_WALKING_LEFT		1
#define MARIO_ANI_JUMPING_LEFT		2

class CMario : public CGameObject
{
public:
	CMario();
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};
