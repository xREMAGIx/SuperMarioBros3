#include <algorithm>
#include "debug.h"

#include "Game.h"
#include "Mario.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt, coObjects);

	if (state == MARIO_STATE_ATTACK) {
		tailAttack->Update(dt, coObjects);
	}

	if (fireball->GetState() == MARIO_FIREBALL_STATE_THROWN) {
		fireball->Update(dt, coObjects);
	}

	if (state == MARIO_STATE_RUNNING || state == MARIO_STATE_HOLDING) {
		vx += (nx / abs(nx)) * MARIO_ACCEL_SPEED;
			if (vx > MARIO_WALKING_SPEED*2 || vx < -MARIO_WALKING_SPEED * 2) {
				vx = (nx / abs(nx)) * MARIO_WALKING_SPEED * 2;
			}
	}
	else {
		vx -= (nx / abs(nx)) * MARIO_DEL_ACCEL_SPEED;
		if (nx > 0) {
			if (vx < 0) {
				vx = 0;
			}
		}
		else {
			if (vx > 0) {
				vx = 0;
			}
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	float current_vy = vy;
	float current_vx = vx;

	coEvents.clear();

	if (deflect_gravity > 0 && vy > 0) {
		vy += (MARIO_GRAVITY - deflect_gravity) * dt;
	}
	else {
		vy += MARIO_GRAVITY * dt;
	}

	//
	// turn off collision when die 
	if (state != MARIO_STATE_DIE && state != MARIO_STATE_GO_CHIMNEY) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (dt_die != 0 && GetTickCount() - dt_die > MARIO_DIE_TIME)
	{
		CBoard* game_board = CBoard::GetInstance();
		game_board->RemoveLives();
		game_board->SetState(BOARD_STATE_IDLE);
		CGame::GetInstance()->SwitchScene(MAP_WOLRD_ID);
	}


	if (dt_attack != 0 && GetTickCount() - dt_attack > MARIO_ATTACK_TIME)
	{
		SetState(MARIO_STATE_IDLE);
		StopAttack();
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

			if (e->obj->GetId() < 0) break;

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
							SetDownLevel();
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
							SetDownLevel();
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
							if (state == MARIO_STATE_RUNNING) {
								x += dx;
								SetCurrentAni(MARIO_ANI_HOLDING);
								greenKoopa->SetState(GREEN_KOOPA_STATE_SHELL_HOLD);
								greenKoopa->SetPosition(x + MARIO_WIDTH + 2, y - 2);
							}
							else {
								SetState(MARIO_STATE_KICK);
								greenKoopa->SetDirection(-this->nx);
								greenKoopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
							}
							break;
						}
						case GREEN_KOOPA_STATE_SHELL_SCROLL:
						case GREEN_KOOPA_STATE_WALKING: {
							SetDownLevel();
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
							case RED_KOOPA_STATE_HOLDED: {
								if (state == MARIO_STATE_HOLDING) {
									redKoopa->SetPosition(x, y - 2);
								}
								break;
							}
							case RED_KOOPA_STATE_SHELL: {
								if (state == MARIO_STATE_RUNNING) {
									SetState(MARIO_STATE_HOLDING);
									redKoopa->SetState(RED_KOOPA_STATE_HOLDED);
								}
								else {
									SetCurrentAni(MARIO_ANI_KICK);
									redKoopa->SetDirection(this->nx);
									redKoopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
								}
								break;
							}
							case RED_KOOPA_STATE_SHELL_SCROLL:
							case RED_KOOPA_STATE_WALKING: {
								SetDownLevel();
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
				CChimney* chimney = dynamic_cast<CChimney*>(e->obj);
				if (e->ny < 0)
				{
					CGameObject* chimney_obj = chimney->GetChimneyObject();
					if (chimney_obj != NULL) {
						if (dynamic_cast<CVenusFireTrap*>(chimney_obj)->GetState()== VENUS_FIRE_TRAP_STATE_SHOW) {
							chimney->SetState(CHIMNEY_STATE_CANCEL);
						}
					}
					if (state == MARIO_STATE_DOWN) {
						if (chimney->GetDirection() == 1 && chimney->GetSceneId() != -1) {
							CPlayScene* scene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
							scene->SetChangeScene(chimney->GetSceneId());
							SetState(MARIO_STATE_GO_CHIMNEY);
						}
					}
					
				}
				else if (e->ny > 0)
				{
					if (up == 1) {
						if (chimney->GetDirection() == -1 && chimney->GetSceneId() != -1) {
							CPlayScene* scene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
							scene->SetChangeScene(chimney->GetSceneId());
							SetState(MARIO_STATE_GO_CHIMNEY);
						}
					}
				}
			}

			//Interact with coin
			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Block 
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				coin->SetState(COIN_STATE_EARNED);

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

			//Interact with venus
			if (dynamic_cast<CVenusFireTrap*>(e->obj)) // if e->obj is Block 
			{
				if (GetUntouchable() == 0 && GetState() != MARIO_STATE_DIE)
				{
					SetDownLevel();
				}
			}

			//Interact with CEnemyWall
			if (dynamic_cast<CEnemyWall*>(e->obj)) // if e->obj is Block 
			{
				this->vy = current_vy;
				this->vx = current_vx;
				y += dy;
				x += dx;
			}

			//Interact with CDeadline
			if (dynamic_cast<CDeadline*>(e->obj)) // if e->obj is Block 
			{
				without_die_jump = 1;
				SetState(MARIO_STATE_DIE);
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
	switch (state)
	{
		case MARIO_STATE_ATTACK: {
			if (level == MARIO_LEVEL_TAIL) {
				current_ani = MARIO_ANI_TAIL_ATTACK;
			}
			else if (level == MARIO_LEVEL_FIRE) {
				current_ani = MARIO_ANI_FIRE_ATTACK;
			}
			break;
		}
		case MARIO_STATE_JUMP:
		case MARIO_STATE_JUMP_RIGHT:
		case MARIO_STATE_JUMP_LEFT:
		{
			if (vx > MARIO_WALKING_SPEED * 1.5 || vx < -MARIO_WALKING_SPEED * 1.5) {
				switch (level)
				{
					case MARIO_LEVEL_FIRE: {
						current_ani = MARIO_ANI_FIRE_FLYING;
						break;
					}
					case MARIO_LEVEL_TAIL: {
						current_ani = MARIO_ANI_TAIL_FLYING;
						break;
					}
					case MARIO_LEVEL_BIG: {
						current_ani = MARIO_ANI_BIG_FLYING;
						break;
					}
					default:
						current_ani = MARIO_ANI_FLYING;
						break;
				}
				break;
			}
			else {
				switch (level)
				{
					case MARIO_LEVEL_FIRE: {
						current_ani = MARIO_ANI_FIRE_JUMPING_LEFT;
						break;
					}	
					case MARIO_LEVEL_TAIL: {
						current_ani = MARIO_ANI_TAIL_JUMPING_LEFT;
						break;
					}
					case MARIO_LEVEL_BIG: {
						current_ani = MARIO_ANI_BIG_JUMPING_LEFT;
						break;
					}
					default:
						current_ani = MARIO_ANI_JUMPING_LEFT;
						break;
				}
				break;
			}
		}
		case MARIO_STATE_WALKING_RIGHT:
		case MARIO_STATE_WALKING_LEFT:
		{
			switch (level)
			{
				case MARIO_LEVEL_FIRE: {
					current_ani = MARIO_ANI_FIRE_WALKING_LEFT;
					break;
				}
				case MARIO_LEVEL_TAIL: {
					current_ani = MARIO_ANI_TAIL_WALKING_LEFT;
					break;
				}
				case MARIO_LEVEL_BIG: {
					current_ani = MARIO_ANI_BIG_WALKING_LEFT;
					break;
				}
				default:
					current_ani = MARIO_ANI_WALKING_LEFT;
					break;
			}
			break;
		}
		case MARIO_STATE_DIE: {
			current_ani = MARIO_ANI_DIE;
			break;
		}
		case MARIO_STATE_KICK: {
			current_ani = MARIO_ANI_KICK;
			break;
		}
		case MARIO_STATE_DOWN: {
			switch (level)
			{
				case MARIO_LEVEL_FIRE: {
					current_ani = MARIO_ANI_FIRE_DOWN;
					break;
				}
				case MARIO_LEVEL_TAIL: {
					current_ani = MARIO_ANI_TAIL_DOWN;
					break;
				}
				case MARIO_LEVEL_BIG: {
					current_ani = MARIO_ANI_BIG_DOWN;
					break;
				}
				default:
					break;
			}
			break;
		}
		case MARIO_STATE_HOLDING: {
			current_ani = MARIO_ANI_HOLDING;
			break;
		}
		default:
			if (vx == 0) {
				switch (level)
				{
					case MARIO_LEVEL_FIRE: {
						current_ani = MARIO_ANI_FIRE_IDLE_LEFT;
						break;
					}
					case MARIO_LEVEL_TAIL: {
						current_ani = MARIO_ANI_TAIL_IDLE_LEFT;
						break;
					}
					case MARIO_LEVEL_BIG: {
						current_ani = MARIO_ANI_BIG_IDLE_LEFT;
						break;
					}
					default:
						current_ani = MARIO_ANI_IDLE_LEFT;
						break;
				}
			}
			else {
				if (vx > MARIO_WALKING_SPEED * 1.5 || vx < -MARIO_WALKING_SPEED * 1.5) {
					switch (level)
						{
							case MARIO_LEVEL_FIRE: {
								current_ani = MARIO_ANI_FIRE_RUNNING;
								break;
							}
							case MARIO_LEVEL_TAIL: {
								current_ani = MARIO_ANI_TAIL_RUNNING;
								break;
							}
							case MARIO_LEVEL_BIG: {
								current_ani = MARIO_ANI_BIG_RUNNING;
								break;
							}
							default:
								current_ani = MARIO_ANI_RUNNING;
								break;
						}
					break;
				}
			}
			break;
	}
	if (state == MARIO_STATE_ATTACK && level == MARIO_LEVEL_TAIL) {
		animation_set->at(current_ani)->Render(x, y, -nx, 255);
		return;
	}
	if (fireball->GetState() == MARIO_FIREBALL_STATE_THROWN) {
		fireball->Render();
	}
	animation_set->at(current_ani)->Render(x, y, nx, 255);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case MARIO_STATE_ATTACK: {
			if (level == MARIO_LEVEL_TAIL) {
				tailAttack->SetPosition(x, y);
				tailAttack->SetDirection(nx);
			}
			else if (level == MARIO_LEVEL_FIRE) {
				fireball = new CMarioFireball();
				fireball->SetPosition(x, y);
				fireball->SetDirection(nx);
				fireball->SetState(MARIO_FIREBALL_STATE_THROWN);
			}
			StartAttack();
			break;
		}
		case MARIO_STATE_WALKING_RIGHT: 
		case MARIO_STATE_JUMP_RIGHT:
		{
			up = 0;
			vx = MARIO_WALKING_SPEED;
			nx = 1;
			break;
		}
		case MARIO_STATE_WALKING_LEFT:
		case MARIO_STATE_JUMP_LEFT:
		{
			up = 0;
			vx = -MARIO_WALKING_SPEED;
			nx = -1;
			break;
		}
		case MARIO_STATE_JUMP:
			up = 0;
			vy = -MARIO_JUMP_SPEED_Y;
			break;
		case MARIO_STATE_DIE: {
			if (without_die_jump != 1) {
				vy = -MARIO_DIE_DEFLECT_SPEED;
			}
			vx = 0;
			StartDie();
			break;
		}
		case MARIO_STATE_DOWN: {
			if (level != MARIO_LEVEL_SMALL) {
				down = 1;
			}
			break;
		}
		case MARIO_STATE_GO_CHIMNEY: {
			vy = MARIO_WALKING_SPEED / 2;
			StartGoChimney();
			break;
		}
		case MARIO_STATE_IDLE: {
			deflect_gravity = 0;
			up = 0;
			if (down == 1) {
				y += MARIO_SMALL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT - 1;
				down = 0;
			}
			break;
		}
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if ((level != MARIO_LEVEL_SMALL) && state != MARIO_STATE_DOWN)
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