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
	cx = camPos.x + BOARD_MARGIN_LEFT;
	cy = camPos.y + game->GetScreenHeight() - BOARD_HEIGHT - BOARD_MARGIN_BOTTOM;
	SetPosition(cx, cy);

	if (state == BOARD_STATE_START &&  GetTickCount() - _count >= 1000)  // 1000 millisecond
	{
		time--;
		_count = GetTickCount();
	}
}


void CBoard::Render()
{
	LPSPRITE background;
	background = CSprites::GetInstance()->Get(40057);
	background->Draw(x - BOARD_MARGIN_LEFT, y - 1);
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(40046);
	sprite->Draw(x, y);

	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scene)->GetPlayer();

	code->DrawNumber(1, x + BOARD_WORLD_ID_X, y + BOARD_WORLD_ID_Y, 1); // world id

	if (mario != NULL && mario->GetState() == MARIO_STATE_RUNNING) {
		for (int i = 0; i < 6; i++)	//arrow
			{
			if (mario->vx > MARIO_WALKING_SPEED * 2 * i / 6 || mario->vx < -MARIO_WALKING_SPEED * 2 * i / 6) {
				code->DrawCharacter(FONT_ARROW_WHITE, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y);
			}
			else {
				code->DrawCharacter(FONT_ARROW_BLACK, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y);
			}
		}
		if (mario->vx >= MARIO_WALKING_SPEED * 2 || mario->vx <= -MARIO_WALKING_SPEED * 2 ) {
			code->DrawCharacter(FONT_P_WHITE, x + BOARD_P_X, y + BOARD_P_Y); //p
		}
		else {
			code->DrawCharacter(FONT_P_BLACK, x + BOARD_P_X, y + BOARD_P_Y); //p
		}
	}
	else {
		for (int i = 0; i < 6; i++) {	//arrow
			code->DrawCharacter(FONT_ARROW_BLACK, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y);
		}
		code->DrawCharacter(FONT_P_BLACK, x + BOARD_P_X, y + BOARD_P_Y); //p
	}
	
	code->DrawCharacter(FONT_PLAY_CHARACTER, x + BOARD_PLAY_CHARACTER_X, y + BOARD_PLAY_CHARACTER_Y); // character
	code->DrawNumber(1, x + BOARD_LIVES_X, y + BOARD_LIVES_Y, lives); // lives
	code->DrawNumber(3, x + BOARD_TIME_X, y + BOARD_TIME_Y, time); // time
	code->DrawNumber(7, x + BOARD_SCORE_X, y + BOARD_SCORE_Y, score); // score
	code->DrawNumber(2, x + BOARD_MONEY_X, y + BOARD_MONEY_Y, money); // money
	
	for (int i = 0; i < 3; i++)	//item boxes
	{
		itemBoxes.at(i)->SetPosition(x + i * ITEM_BOX_WIDTH + BOARD_ITEM_BOX_X, y + BOARD_ITEM_BOX_Y);
		itemBoxes.at(i)->Render();
	}
}

void CBoard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CBoard::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOARD_STATE_START:
		StartCount();
		break;
	case BOARD_STATE_IDLE:
		StopCount();
		break;
	default: 
		break;
	}
}
