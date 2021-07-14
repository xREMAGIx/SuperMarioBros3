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

	if (GetTickCount() - _count >= 1000)  // 1000 millisecond
	{
		time--;
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

	code->DrawNumber(1, x + BOARD_WORLD_ID_X, y + BOARD_WORLD_ID_Y, 1); // world id

	for (int i = 0; i < 6; i++)	//arrow
	{
		code->DrawCharacter(FONT_ARROW_BLACK, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y); // world id
	}
	code->DrawCharacter(FONT_P_BLACK, x + BOARD_P_X, y + BOARD_P_Y); //p

	code->DrawCharacter(FONT_PLAY_CHARACTER, x + BOARD_PLAY_CHARACTER_X, y + BOARD_PLAY_CHARACTER_Y); // character
	code->DrawNumber(2, x + BOARD_LIVES_X, y + BOARD_LIVES_Y, 25); // lives
	code->DrawNumber(3, x + BOARD_TIME_X, y + BOARD_TIME_Y, time); // time
	code->DrawNumber(7, x + BOARD_SCORE_X, y + BOARD_SCORE_Y, score); // score
}

void CBoard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}