#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Font.h"
#include "ItemBox.h"
#include "Mario.h"
#include "debug.h"

//NOTE BOARD START POINT	12	246	(REMEMER MINUS 1)

#define BOARD_MARGIN_BOTTOM 16
#define BOARD_MARGIN_LEFT 2
#define BOARD_HEIGHT 36
#define BOARD_BACKGROUND_WIDTH	252
#define BOARD_UI_WIDTH	150

#define BOARD_STATE_IDLE 0
#define BOARD_STATE_START 1
#define BOARD_STATE_STOP 2

#define BOARD_TIME_X 128
#define BOARD_TIME_Y 4
#define BOARD_SCORE_X 56
#define BOARD_SCORE_Y 4
#define BOARD_PLAY_CHARACTER_X 10
#define BOARD_PLAY_CHARACTER_Y 4
#define BOARD_LIVES_X 40
#define BOARD_LIVES_Y 4
#define BOARD_WORLD_ID_X 40
#define BOARD_WORLD_ID_Y 7
#define BOARD_ARROWS_X 56
#define BOARD_ARROWS_Y -4
#define BOARD_P_X 108
#define BOARD_P_Y -4
#define BOARD_MONEY_X 136
#define BOARD_MONEY_Y -4
#define BOARD_ITEM_BOX_X 172
#define BOARD_ITEM_BOX_Y 0

#define BOARD_TIME_COUNTDOWN_SECOND	1000

#define BOARD_WORLD_ID_CHAR	1
#define BOARD_LIVES_CHAR	1
#define BOARD_TIME_CHAR	3
#define BOARD_SCORE_CHAR	7
#define BOARD_MONEY_CHAR	2

#define BOARD_NUMBER_OF_ARROWS	6
#define BOARD_NUMBER_OF_ITEM_BOXES	3

class CBoard : public CGameObject
{
	static CBoard* __instance;

	int time;
	int score;
	int lives;
	int worldId;
	int money;

	ULONGLONG _count;
	CFont* code;
	vector<CItemBox*> itemBoxes;

public:
	static CBoard* GetInstance();
	CBoard()
	{
		_count = 0;
		worldId = 0;
		time = 0;
		lives = 4;
		code = new CFont();
		score = 0;
		money = 0;
		itemBoxes.push_back(new CItemBox());
		itemBoxes.push_back(new CItemBox());
		itemBoxes.push_back(new CItemBox());
	}
	void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

	void StartCount() { time = 300; _count = GetTickCount64(); }
	void StopCount() { time = 0; }
	int GetLives() { return lives; }
	void ResetLives() { lives = 4; }
	void AddLives() { lives++; }
	void RemoveLives() { lives--; }
	void AddMoney() { money++; }
	void AddPoint(int point) { score += point; }
};

typedef CBoard* LPGAMEBOARD;