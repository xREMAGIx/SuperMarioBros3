#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Font.h"

//NOTE BOARD START POINT	12	246	(REMEMER MINUS 1)

#define BLOCK_BBOX_WIDTH  16
#define BOARD_HEIGHT 30

#define BOARD_TIME_X 123
#define BOARD_TIME_Y 15

#define BOARD_SCORE_X 52
#define BOARD_SCORE_Y 15

#define BOARD_PLAY_CHARACTER_X 4
#define BOARD_PLAY_CHARACTER_Y 15

#define BOARD_LIVES_X 29
#define BOARD_LIVES_Y 15

#define BOARD_WORLD_ID_X 37
#define BOARD_WORLD_ID_Y 7

#define BOARD_ARROWS_X 52
#define BOARD_ARROWS_Y 7

#define BOARD_P_X 101
#define BOARD_P_Y 7

class CBoard : public CGameObject
{
	static CBoard* __instance;

	int time;
	int score;
	int worldId;
	DWORD _count;
	CFont* code;

public:
	static CBoard* GetInstance();
	CBoard()
	{
		worldId = 0;
		time = 300;
		_count = GetTickCount();
		code = new CFont();
		score = 0;
	}
	void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};