#include "GameBoard.h"

CBoard* CBoard::__instance = NULL;

CBoard* CBoard::GetInstance()
{
	if (__instance == NULL) __instance = new CBoard();
	return __instance;
}

void CBoard::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	float cx, cy;
	D3DXVECTOR2 camPos = game->GetCamPos();
	cx = camPos.x;
	cy = camPos.y + game->GetScreenHeight() - BOARD_HEIGHT;
	SetPosition(cx, cy);

	code->DrawNumber(3, x + BOARD_TIME_X, y + BOARD_TIME_Y, _time); // time


	if (GetTickCount() - _count >= 1000)  // 1000 millisecond
	{
		_time--;
		_count = GetTickCount();
	}
}


void CBoard::Render()
{
	CGame* game = CGame::GetInstance();
	float cx, cy;
	D3DXVECTOR2 camPos = game->GetCamPos();
	cx = camPos.x;
	cy = camPos.y + game->GetScreenHeight() - BOARD_HEIGHT;

	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(40046);
	sprite->Draw(x, y);

	code->DrawNumber(3, x + BOARD_TIME_X, y + BOARD_TIME_Y, _time); // time
}

void CBoard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}