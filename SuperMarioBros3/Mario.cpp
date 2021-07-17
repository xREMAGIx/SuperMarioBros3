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

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	float current_vy = vy;
	float current_vx = vx;

	coEvents.clear();

	// Simple fall down
	vy += MARIO_GRAVITY * dt;


	// turn off collision when die 
	if (state != MARIO_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (dt_die!=0 && GetTickCount() - dt_die > MARIO_DIE_TIME)
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
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Interact with CEnemyWall
			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				this->vy = current_vy;
				this->vx = current_vx;
				y += dy;
				x += dx;
			}

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (e->nx != 0) {
					this->vx = current_vx;
					x += dx;
				}

				if (e->ny > 0)
				{
					this->vy = current_vy;
					y += dy;
				}
			}
			

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
					case PARA_GOOMBA_STATE_JUMP_SMALL:
					case PARA_GOOMBA_STATE_JUMP_BIG:
					case PARA_GOOMBA_STATE_WALKING: {
						goomba->SetState(PARA_GOOMBA_STATE_WALKING_WITHOUT_WING);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						break;
					}
					case PARA_GOOMBA_STATE_WALKING_WITHOUT_WING: {
						goomba->SetState(PARA_GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						break;
					}
					default: {
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
					switch (greenKoopa->GetState())
					{
					case GREEN_KOOPA_STATE_SHELL: {
						greenKoopa->SetState(GREEN_KOOPA_STATE_SHELL_SCROLL);
						break;
					}
					default: {
						greenKoopa->SetState(GREEN_KOOPA_STATE_SHELL);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						break;
					}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						switch (greenKoopa->GetState())
						{
						case GREEN_KOOPA_STATE_SHELL: {
							greenKoopa->SetDirection(-nx);
							greenKoopa->SetState(GREEN_KOOPA_STATE_SHELL_SCROLL);
							break;
						}
						case GREEN_KOOPA_STATE_SHELL_SCROLL:
						case GREEN_KOOPA_STATE_WALKING: {
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
							case RED_KOOPA_STATE_SHELL_SCROLL:
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
				CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
				if (e->ny > 0)
				{
					if (questionBlock->GetState() == QUESTION_BLOCK_STATE_IDLE)
					{
						questionBlock->SetState(QUESTION_BLOCK_STATE_OPENED);
					}
				}
			}


			//Interact with chimney
			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
			}

			//Interact with coin
			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Block 
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				coin->SetState(COIN_STATE_EARNED);
			}
			//Jump touch music box
			
			if (dynamic_cast<CMusicBox*>(e->obj)) // if e->obj is Block 
			{
				CMusicBox* block = dynamic_cast<CMusicBox*>(e->obj);
			}
			
			if (state != MARIO_STATE_DIE) {
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani;
	switch (state)
	{
		case MARIO_STATE_JUMP:
		case MARIO_STATE_JUMP_RIGHT:
		case MARIO_STATE_JUMP_LEFT:
		{
			if (level == MARIO_LEVEL_BIG) {
				ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
			else {
				ani = MARIO_ANI_JUMPING_LEFT;
			}
			break;
		}
		case MARIO_STATE_WALKING_RIGHT:
		case MARIO_STATE_WALKING_LEFT:
		{
			if (level == MARIO_LEVEL_BIG) {
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
			else {
				ani = MARIO_ANI_WALKING_LEFT;
			}
			break;
		}
		case MARIO_STATE_DIE: {
			ani = MARIO_ANI_DIE;
			break;
		}
		default:
			if (level == MARIO_LEVEL_BIG) {
				ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else {
				ani = MARIO_ANI_IDLE_LEFT;
			}
			break;
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
		case MARIO_STATE_DIE: {
			vy = -MARIO_DIE_DEFLECT_SPEED;
			vx = 0;
			StartDie();
			break;
		}
		case MARIO_STATE_IDLE: {
			vx = 0;
			break;
		}
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
	else if(state != MARIO_STATE_DIE)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	} else {
		right = x + 1;
		bottom = y + 1;
	}
}