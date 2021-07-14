#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Font.h"

#define BLOCK_BBOX_WIDTH  16
#define BOARD_HEIGHT 30

#define BOARD_TIME_X 123
#define BOARD_TIME_Y 15

class CBoard : public CGameObject
{
	static CBoard* __instance;

	int _time;
	DWORD _count;
	CFont* code;

public:
	static CBoard* GetInstance();
	CBoard()
	{
		_time = 300;
		_count = GetTickCount();
		code = new CFont();
	}
	void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};