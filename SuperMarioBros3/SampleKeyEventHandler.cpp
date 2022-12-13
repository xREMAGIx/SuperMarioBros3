#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	if (!mario) {
		LPPLAYERFONT choosePlayer = (LPPLAYERFONT)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetChoosePlayer();

		switch (KeyCode)
		{
			case DIK_DOWN:
				choosePlayer->SetState(PLAYER_FONT_STATE_TWO);
				break;
			case DIK_UP:
				choosePlayer->SetState(PLAYER_FONT_STATE_ONE);
				break;
			case DIK_S: {
				int sceneId = choosePlayer->GetSceneId();
				CGame::GetInstance()->InitiateSwitchScene(sceneId);
				break;
			}
		}
	}
	else {
		switch (KeyCode)
		{
		case DIK_DOWN:
			mario->SetState(MARIO_STATE_SIT);
			break;
		case DIK_A:
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
				mario->SetState(MARIO_STATE_TAIL_ATTACK);
			}
			break;
		case DIK_S:
			mario->SetState(MARIO_STATE_JUMP);
			break;
		case DIK_1:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			break;
		case DIK_2:
			mario->SetLevel(MARIO_LEVEL_BIG);
			break;
		case DIK_3:
			mario->SetLevel(MARIO_LEVEL_RACCOON);
			break;
		case DIK_0:
			mario->SetState(MARIO_STATE_DIE);
			break;
		case DIK_R: // reset
			//Reload();
			break;
		}
	}
	
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (!mario) return;

	switch (KeyCode)
	{
		case DIK_S:
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
			break;
		case DIK_DOWN:
			mario->SetState(MARIO_STATE_SIT_RELEASE);
			break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (!mario) return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A)) {
			if (!mario->IsTailAttacking()) {
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			}
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A)) {
			if (!mario->IsTailAttacking()) {
				mario->SetState(MARIO_STATE_RUNNING_LEFT);
			}
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}