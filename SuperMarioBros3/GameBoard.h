#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Font.h"
#include "ItemBox.h"


//NOTE BOARD START POINT	12	246	(REMEMER MINUS 1)

#define BOARD_MARGIN_BOTTOM 40
#define BOARD_MARGIN_LEFT 2
#define BOARD_HEIGHT 30

#define BOARD_STATE_IDLE 0
#define BOARD_STATE_START 1
#define BOARD_STATE_STOP 2

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
#define BOARD_MONEY_X 132
#define BOARD_MONEY_Y 7
#define BOARD_ITEM_BOX_X 161
#define BOARD_ITEM_BOX_Y 0

class CBoard : public CGameObject
{
	static CBoard* __instance;

	int time;
	int score;
	int lives;
	int worldId;
	int money;

	DWORD _count;
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

	void StartCount() { time = 300; _count = GetTickCount(); }
	void StopCount() { time = 0; }
	int GetLives() { return lives; }
	void ResetLives() { lives = 4 ; }
	void AddLives() { lives++; }
	void RemoveLives() { lives--; }
	void AddMoney() { money++; }
	void AddPoint(int point) { score += point; }
};