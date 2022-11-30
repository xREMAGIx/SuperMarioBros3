#include "GameBoard.h"
#include "PlayScene.h"

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

	float camX, camY;

	game->GetCamPos(camX, camY);
	cx = camX;
	cy = camY + SCREEN_HEIGHT - BOARD_HEIGHT - BOARD_MARGIN_BOTTOM;
	SetPosition(cx, cy);

	if (state == BOARD_STATE_START && GetTickCount64() - _count >= 1000)  // 1000 millisecond
	{
		time--;
		_count = GetTickCount64();
	}
}


void CBoard::Render()
{
	CSprites* sprites = CSprites::GetInstance();

	sprites->Get(ID_SPRITE_BOARD_BACKGROUND)->Draw(x + BOARD_BACKGROUND_WIDTH / 2, y);

	sprites->Get(ID_SPRITE_BOARD_UI)->Draw(x + BOARD_UI_WIDTH / 2, y);

	code->DrawNumber(1, x + BOARD_WORLD_ID_X, y - FONT_HEIGHT / 2, 1); // world id
	
	 
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
	if (dynamic_cast<CPlayScene*>(scene))
	{
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
		LPGAMEOBJECT mario = playScene->GetPlayer();

		float marioVx, marioVy;

		mario->GetSpeed(marioVx, marioVy);

		if (mario != NULL && (mario->GetState() == MARIO_STATE_RUNNING_RIGHT || mario->GetState() == MARIO_STATE_RUNNING_LEFT)) {
			for (int i = 0; i < 6; i++)	//arrow
			{
				if (marioVx > MARIO_WALKING_SPEED * 2 * i / 6 || marioVx < -MARIO_WALKING_SPEED * 2 * i / 6) {
					code->DrawCharacter(ID_SPRITE_FONT_ARROW_WHITE, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y);
				}
				else {
					code->DrawCharacter(ID_SPRITE_FONT_ARROW_BLACK, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y);
				}
			}
			if (marioVx >= MARIO_WALKING_SPEED * 2 || marioVx <= -MARIO_WALKING_SPEED * 2) {
				code->DrawCharacter(ID_SPRITE_FONT_P_WHITE, x + BOARD_P_X, y + BOARD_P_Y); //p
			}
			else {
				code->DrawCharacter(ID_SPRITE_FONT_P_BLACK, x + BOARD_P_X, y + BOARD_P_Y); //p
			}
		}
		else {
			for (int i = 0; i < 6; i++) {	//arrow
				code->DrawCharacter(ID_SPRITE_FONT_ARROW_BLACK, x + i * 8 + BOARD_ARROWS_X, y + BOARD_ARROWS_Y);
			}
			code->DrawCharacter(ID_SPRITE_FONT_P_BLACK, x + BOARD_P_X, y + BOARD_P_Y); //p
		}
	}
	
	code->DrawCharacter(ID_SPRITE_FONT_PLAY_CHARACTER, x + BOARD_PLAY_CHARACTER_X, y + BOARD_PLAY_CHARACTER_Y); // character
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
