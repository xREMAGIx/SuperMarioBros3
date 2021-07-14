#include <algorithm>
#include "debug.h"

#include "Game.h"
#include "Mario.h"
#include "Goomba.h"
#include "Block.h"
#include "Brick.h"
#include "QuestionBlock.h"
#include "InvisibleBlock.h"
#include "Cloud.h"
#include "GreenKoopa.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (GetTickCount() - dt_die > MARIO_DIE_TIME && dt_die !=0 )
	{
		CBoard* game_board = CBoard::GetInstance();
		game_board->RemoveLives();
		game_board->SetState(BOARD_STATE_IDLE);
		CGame::GetInstance()->SwitchScene(MAP_WOLRD_ID);
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.5f;

		//Collision
		/*
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		*/

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() == GOOMBA_STATE_WALKING)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						goomba->StartDie();
						SetState(MARIO_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() == GOOMBA_STATE_WALKING)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else {
								DebugOut(L"[INFO] Touch Goomba Die\n");
								SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
			}

			//PARA GOOMBA
			if (dynamic_cast<CParaGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					switch (goomba->GetState())
					{
					case PARA_GOOMBA_STATE_WALKING: {
						goomba->SetState(PARA_GOOMBA_STATE_WALKING_WITHOUT_WING);
						SetState(MARIO_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						break;
					}
					case PARA_GOOMBA_STATE_WALKING_WITHOUT_WING: {
						goomba->SetState(PARA_GOOMBA_STATE_DIE);
						SetState(MARIO_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						break;
					}
					default: {
						goomba->SetState(RED_KOOPA_STATE_SHELL);
						SetState(MARIO_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						break;
					}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != PARA_GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else {
								DebugOut(L"[INFO] Touch Goomba Die\n");
								SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
			}

			if (dynamic_cast<CGreenKoopa*>(e->obj)) // if e->obj is Goomba 
			{
				CGreenKoopa* greenKoopa = dynamic_cast<CGreenKoopa*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (greenKoopa->GetState() != GREEN_KOOPA_STATE_DIE)
					{
						greenKoopa->SetState(GREEN_KOOPA_STATE_DIE);
						SetState(MARIO_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (greenKoopa->GetState() != GREEN_KOOPA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}

			if (dynamic_cast<CRedKoopa*>(e->obj)) // if e->obj is Goomba 
			{
	
				CRedKoopa* redKoopa = dynamic_cast<CRedKoopa*>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					switch (redKoopa->GetState())
					{
						case RED_KOOPA_STATE_SHELL: {
							redKoopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
							break;
						}
						default: {
							redKoopa->SetState(RED_KOOPA_STATE_SHELL);
							SetState(MARIO_STATE_IDLE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							break;
						}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						switch (redKoopa->GetState())
						{
							case RED_KOOPA_STATE_SHELL: {
								redKoopa->SetDirection(-nx);
								redKoopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
								break;
							}
							case RED_KOOPA_STATE_WALKING: {
								if (level > MARIO_LEVEL_SMALL)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else {
									DebugOut(L"[INFO] Touch RedGoomba Die\n");
									SetState(MARIO_STATE_DIE);
								}
								break;
							}
						default:
							break;
						}
					}
				}
			}

			//Touch Question Block
			if (dynamic_cast<CQuestionBlock*>(e->obj)) 
			{
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
				if (e->ny > 0)
				{
					if (questionBlock->GetState() == QUESTION_BLOCK_STATE_IDLE)
					{
						questionBlock->SetState(QUESTION_BLOCK_STATE_OPENED);
					}
				}
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
				
			}

			//Jump touch block
			if (dynamic_cast<CBlock*>(e->obj)) // if e->obj is Block 
			{
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				CBlock* block = dynamic_cast<CBlock*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
			}

			//Jump touch block
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Block 
			{
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				CBrick* block = dynamic_cast<CBrick*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
			}

			//Jump touch block
			if (dynamic_cast<CCloud*>(e->obj)) // if e->obj is Block 
			{
				CCloud* block = dynamic_cast<CCloud*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
			}


			//Interact with CEnemyWall
			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				x += dx;
				y += dy;
			}

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					vy = 0;
					SetState(MARIO_STATE_IDLE);
				}

				if (e->ny > 0 || e->nx != 0)
				{
					x += dx;
					y += dy;
				}
			}


			//Touch invisible wall
			if (dynamic_cast<CInvisibleWall*>(e->obj)) // if e->obj is Block 
			{
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				CInvisibleWall* block = dynamic_cast<CInvisibleWall*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					vy = 0;
					SetState(MARIO_STATE_IDLE);
				}
			}

			//Interact with chimney
			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
				CChimney* block = dynamic_cast<CChimney*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
			}

			//Interact with coin
			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Block 
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				coin->SetState(COIN_STATE_EARNED);
			}

			//Interact with mushroom
			if (dynamic_cast<CMushroom*>(e->obj)) // if e->obj is Block 
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
				CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
				SetState(MARIO_LEVEL_BIG);
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani;
	switch (level)
	{
		case MARIO_LEVEL_BIG: {
			if (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT)
			{
				ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
			else if (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_WALKING_LEFT) {
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
			else {
				ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			break;
		}
		default: {
			if (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT)
			{
				ani = MARIO_ANI_JUMPING_LEFT;
			}
			else if (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_WALKING_LEFT) {
				ani = MARIO_ANI_WALKING_LEFT;
			}
			else {
				ani = MARIO_ANI_IDLE_LEFT;
			}
			break;
		}
	}
	if (state == MARIO_STATE_DIE) {
		ani = MARIO_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y, nx, 255);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case MARIO_STATE_WALKING_RIGHT: 
		case MARIO_STATE_JUMP_RIGHT:
		{
			vx = MARIO_WALKING_SPEED;
			nx = 1;
			break;
		}
		case MARIO_STATE_WALKING_LEFT:
		case MARIO_STATE_JUMP_LEFT:
		{
			vx = -MARIO_WALKING_SPEED;
			nx = -1;
			break;
		}
		case MARIO_STATE_JUMP:
			vy = -MARIO_JUMP_SPEED_Y;
			break;
		case MARIO_STATE_DIE:
			vx = 0;
			vy = -MARIO_DIE_DEFLECT_SPEED;
			StartDie();
			break;
		case MARIO_STATE_IDLE:
			vx = 0;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}