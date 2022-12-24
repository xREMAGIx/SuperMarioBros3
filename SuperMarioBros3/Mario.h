#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f
#define MARIO_JUMP_TAIL_SPEED_Y		0.9f

#define MARIO_GRAVITY			0.002f
#define MARIO_TAIL_GRAVITY			0.0002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	1
#define MARIO_STATE_WALKING_LEFT	2

#define MARIO_STATE_JUMP			3
#define MARIO_STATE_RELEASE_JUMP    4

#define MARIO_STATE_RUNNING_RIGHT	5
#define MARIO_STATE_RUNNING_LEFT	6

#define MARIO_STATE_SIT				7
#define MARIO_STATE_SIT_RELEASE		8

#define	MARIO_STATE_TAIL_ATTACK	9
#define MARIO_STATE_TAIL_ATTACK_RELEASE		10

#pragma region ANIMATION_ID

// SMALL MARIO


#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON	3

#define MARIO_BIG_BBOX_WIDTH  18
#define MARIO_BIG_BBOX_HEIGHT 28
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 18

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  16
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_RACCOON_BBOX_WIDTH	24
#define MARIO_RACCOON_BBOX_HEIGHT	28
#define MARIO_RACCOON_SITTING_BBOX_WIDTH  22
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 18

#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_TAIL_ATTACK_TIME 300
#define MARIO_TAIL_JUMP_TIME 300

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isOnPlatform;
	BOOLEAN isTailAttacking;
	BOOLEAN isTailJumping;

	DWORD updateDt;

	float current_vy = vy;
	float current_vx = vx;

	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 

	ULONGLONG untouchable_start;
	ULONGLONG tail_attack_start;
	ULONGLONG tail_jump_start;

	void OnCollisionWithTopPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithRedGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithGreenMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		tail_attack_start = -1;
		tail_jump_start = -1;
		isOnPlatform = false;
		isTailAttacking = false;
		isTailJumping = false;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	BOOLEAN IsTailAttacking() { return isTailAttacking; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetLevel() {
		return this->level;
	}
	void SetLevel(int l);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};