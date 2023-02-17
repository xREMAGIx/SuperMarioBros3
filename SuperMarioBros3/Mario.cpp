#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Mushroom.h"
#include "GreenMushroom.h"
#include "QuestionBlock.h"
#include "Coin.h"
#include "Portal.h"
#include "TopPlatform.h"
#include "SuperLeaf.h"
#include "VenusFireTrap.h"
#include "Deadline.h"

#include "Collision.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == MARIO_STATE_DIE) {
		vy += ay * dt;
		y += vy * dt;

		CGame* game = CGame::GetInstance();
		float minX, minY;
		game->GetMinCamScreen(minX, minY);

		int screen_width = game->GetScreenWidth();

		if (y > minY + screen_width) {
			vy = 0;
			y = minY + screen_width;
		}

		if (GetTickCount64() - die_start > MARIO_DIE_TIME)
		{
			CBoard* game_board = CBoard::GetInstance();
			game_board->RemoveLives();
			game_board->SetState(BOARD_STATE_IDLE);

			CGame* game = CGame::GetInstance();

			if (game_board->GetLives() == 0) {
				game_board->ResetLives();
				CPlayScene* scene = (CPlayScene*)(game->GetCurrentScene());
				scene->SetCurrentMapPoint(MARIO_DIE_BACK_MAP_POINT);
				game->InitiateSwitchScene(MARIO_DIE_BACK_SCENE);
			}
			else {
				int previousId = game->GetPreviousSceneId();
				game->InitiateSwitchScene(previousId);
			}
		}
		return;
	}

	if (state == MARIO_STATE_PORTAL && collidedPortal->GetSceneId() != -1) {
		if (GetTickCount64() - portal_start > MARIO_PORTAL_TIME) {
			CGame* game = CGame::GetInstance();
			CPlayScene* scene = (CPlayScene*)(game->GetCurrentScene());

			float posX, posY;
			collidedPortal->GetNextPos(posX, posY);
			int next_direction = collidedPortal->GetNextPortalDirection();

			scene->SetPortalNextPos(posX, posY, next_direction);
			scene->SetCurrentMarioLevel(level);
			game->InitiateSwitchScene(collidedPortal->GetSceneId());
		}
		
		RunPortalMovement(dt);
		return;
	}

	if (state == MARIO_STATE_PORTAL_OUT) {
		if (GetTickCount64() - portal_out_start > MARIO_PORTAL_OUT_TIME) {
			ay = MARIO_GRAVITY;
			vy = 0;
			state = MARIO_STATE_IDLE;
		}

		RunPortalMovement(dt);
		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) {
		vx = maxVx;
	}

	current_vy = vy;
	current_vx = vx;

	if (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT) {
		if (GetTickCount64() - accel_increase_start > MARIO_ACCEL_INCREASE_TIME) {
			if (accelPoint < MARIO_ACCEL_POWER_X) {
				accelPoint++;
			}
			StartAccelIncrease();
		}
	}
	else {
		if (accelPoint == MARIO_ACCEL_POWER_X && isTailJumping) {
			if (GetTickCount64() - accel_increase_start > MARIO_ACCEL_INCREASE_TIME) {
				if (accelPoint < MARIO_ACCEL_POWER_X) {
					accelPoint++;
				}
				StartAccelIncrease();
			}
		} else if (accelPoint > 0 && GetTickCount64() - accel_increase_start > MARIO_ACCEL_INCREASE_TIME) {
			if (accelPoint > 0) {
				accelPoint--;
			}
			StartAccelIncrease();
		}
	}

	if (holdingObject != NULL) {
		if (dynamic_cast<CRedKoopa*>(holdingObject)) {
			CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(holdingObject);

			if (koopa->GetState() == RED_KOOPA_STATE_WALKING) {
				SetHoldingObject(NULL);
			}
			else {
				float holdingObjectX, holdingObjectY;
				int width = MARIO_SMALL_BBOX_WIDTH;
				if (level == MARIO_LEVEL_BIG) {
					width = MARIO_BIG_BBOX_WIDTH;
				}
				else if (level == MARIO_LEVEL_RACCOON) {
					width = MARIO_RACCOON_BBOX_WIDTH;
				}
				holdingObjectX = x + nx * (width / 2 + 4);
				holdingObjectY = y + ny * 4;
				koopa->SetPosition(holdingObjectX, holdingObjectY);
			}
		} else if (dynamic_cast<CKoopa*>(holdingObject)) {
			CKoopa* koopa = dynamic_cast<CKoopa*>(holdingObject);

			if (koopa->GetState() == KOOPA_STATE_WALKING) {
				SetHoldingObject(NULL);
			}
			else {
				float holdingObjectX, holdingObjectY;
				int width = MARIO_SMALL_BBOX_WIDTH;
				if (level == MARIO_LEVEL_BIG) {
					width = MARIO_BIG_BBOX_WIDTH;
				}
				else if (level == MARIO_LEVEL_RACCOON) { 
					width = MARIO_RACCOON_BBOX_WIDTH;
				}
				holdingObjectX = x + nx * width;
				holdingObjectY = y + ny * 4;
				koopa->SetPosition(holdingObjectX, holdingObjectY);
			}
		}
	}

	// reset untouchable timer if untouchable time has passed
	if (isUntouchable)
	{
		if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) {
			untouchable_start = 0;
			isUntouchable = false;
			flickering_start = 0;
		}

		if (GetTickCount64() - flickering_start > MARIO_FLICKERING_TIME) {
			isFlickering = !isFlickering;
			flickering_start = GetTickCount64();
		}
	}

	// reset tail attack
	if (isTailAttacking)
	{
		tail->Update(dt, coObjects);
		if (GetTickCount64() - tail_attack_start > MARIO_TAIL_ATTACK_TIME) {
			SetState(MARIO_STATE_TAIL_ATTACK_RELEASE);
		}
	}

	//reset tail jump
	if (isTailJumping && (GetTickCount64() - tail_jump_start > MARIO_TAIL_JUMP_TIME))
	{
		isTailJumping = false;
		tail_jump_start = 0;
		SetState(MARIO_STATE_RELEASE_JUMP);
	}

	collidedPortal = NULL;
	renderOrder = 1;

	isOnPlatform = false;
	updateDt = dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}
	
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CRedGoomba*>(e->obj))
		OnCollisionWithRedGoomba(e);
	else if (dynamic_cast<CChimney*>(e->obj))
		OnCollisionWithChimney(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CRedKoopa*>(e->obj))
		OnCollisionWithRedKoopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CGreenMushroom*>(e->obj))
		OnCollisionWithGreenMushroom(e);
	else if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
	else if (dynamic_cast<CVenusFireTrap*>(e->obj))
		OnCollisionWithVenusFireTrap(e);
	else if (dynamic_cast<CFireball*>(e->obj))
		OnCollisionWithFireBall(e);
	else if (dynamic_cast<CSuperLeaf*>(e->obj))
		OnCollisionWithSuperLeaf(e);
	else if (dynamic_cast<CDeadline*>(e->obj))
		OnCollisionWithDeadline(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CPSwitch*>(e->obj))
		OnCollisionWithPSwitch(e);
	else if (dynamic_cast<CGoal*>(e->obj))
		OnCollisionWithGoal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			ay = MARIO_GRAVITY;
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (isTailAttacking && nAttacking != e->nx) {
			if (goomba->GetState() != GOOMBA_STATE_DIE || goomba->GetState() != GOOMBA_STATE_JUMP_DIE) {
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
			}
		}
		else if (holdingObject) {
			if (nx > 0 && e->nx < 0 || nx < 0 && e->nx > 0) {
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
				if (dynamic_cast<CRedKoopa*>(this->holdingObject)) {
					CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(this->holdingObject);
					koopa->SetIsHolded(false);
					koopa->SetState(RED_KOOPA_STATE_JUMP_DIE);
				}
				else if (dynamic_cast<CKoopa*>(this->holdingObject)) {
					CKoopa* koopa = dynamic_cast<CKoopa*>(this->holdingObject);
					koopa->SetIsHolded(false);
					koopa->SetState(KOOPA_STATE_JUMP_DIE);
				}
				SetHoldingObject(NULL);
			}
		}
		else {
			if (!isUntouchable)
			{
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					DecreaseLevel();
				}
			}
		}
	}
}

void CMario::OnCollisionWithRedGoomba(LPCOLLISIONEVENT e)
{
	CRedGoomba* goomba = dynamic_cast<CRedGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != RED_GOOMBA_STATE_DIE)
		{
			if (goomba->GetIsHaveWing()) {
				goomba->SetIsHaveWing(false);
			}
			else {
				goomba->SetState(RED_GOOMBA_STATE_DIE);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (isTailAttacking && nAttacking != e->nx) {
			if (goomba->GetState() != RED_GOOMBA_STATE_DIE || goomba->GetState() != RED_GOOMBA_STATE_JUMP_DIE) {
				goomba->SetState(RED_GOOMBA_STATE_JUMP_DIE);
			}
		}
		else if (holdingObject) {
			if (nx > 0 && e->nx < 0 || nx < 0 && e->nx > 0) {
				goomba->SetState(RED_GOOMBA_STATE_JUMP_DIE);
				if (dynamic_cast<CRedKoopa*>(this->holdingObject)) {
					CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(this->holdingObject);
					koopa->SetIsHolded(false);
					koopa->SetState(RED_KOOPA_STATE_JUMP_DIE);
				}
				else if (dynamic_cast<CKoopa*>(this->holdingObject)) {
					CKoopa* koopa = dynamic_cast<CKoopa*>(this->holdingObject);
					koopa->SetIsHolded(false);
					koopa->SetState(KOOPA_STATE_JUMP_DIE);
				}
				SetHoldingObject(NULL);
			}
		}
		else if (!isUntouchable)
		{
			if (goomba->GetState() != RED_GOOMBA_STATE_DIE)
			{
				DecreaseLevel();
			}
		}
	}
}

void CMario::OnCollisionWithChimney(LPCOLLISIONEVENT e)
{
	CChimney* chimney = dynamic_cast<CChimney*>(e->obj);

	if (e->ny < 0)
	{
		CGameObject* object = chimney->GetChimneyObject();
		if (object) {
			if (dynamic_cast<CVenusFireTrap*>(object))
			{
				if (object->GetState() == VENUS_FIRE_TRAP_STATE_SHOW) {
					object->SetState(VENUS_FIRE_TRAP_STATE_SHOW);
				}
			}
		}
	}
}

void CMario::OnCollisionWithVenusFireTrap(LPCOLLISIONEVENT e)
{
	CVenusFireTrap* venusFireTrap = dynamic_cast<CVenusFireTrap*>(e->obj);

	if (!isUntouchable)
	{
		if (isTailAttacking && nAttacking != e->nx) {
			if (venusFireTrap->GetState() != VENUS_FIRE_TRAP_STATE_JUMP_DIE || venusFireTrap->GetState() != VENUS_FIRE_TRAP_STATE_DIE) {
				venusFireTrap->SetState(VENUS_FIRE_TRAP_STATE_JUMP_DIE);
			}
		}
		else if (venusFireTrap->GetState() != VENUS_FIRE_TRAP_STATE_DIE)
		{
			DecreaseLevel();
		}
	}
}

void CMario::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{
	CFireball* fireball = dynamic_cast<CFireball*>(e->obj);

	DecreaseLevel();

	fireball->SetState(FIREBALL_STATE_DESTROYED);
}

void CMario::OnCollisionWithRedKoopa(LPCOLLISIONEVENT e)
{
	CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(e->obj);

	if (koopa->GetIsHolded()) return;

	// jump on top
	if (e->ny < 0 && !this->isOnPlatform)
	{
		if (koopa->GetState() != RED_KOOPA_STATE_SHELL)
		{
			koopa->SetState(RED_KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			if (nx > 0) {
				koopa->SetSpeed(RED_KOOPA_SHELL_SCROLL_SPEED, 0);
			}
			else {
				koopa->SetSpeed(-RED_KOOPA_SHELL_SCROLL_SPEED, 0);
			}
			koopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
		}
	}
	else // hit by
	{
		if (holdingObject) {
			if (nx > 0 && e->nx < 0 || nx < 0 && e->nx > 0) {
				koopa->SetState(RED_KOOPA_STATE_JUMP_DIE);
				if (dynamic_cast<CRedKoopa*>(this->holdingObject)) {
					CRedKoopa* koopaOther = dynamic_cast<CRedKoopa*>(this->holdingObject);
					koopaOther->SetIsHolded(false);
					koopaOther->SetState(RED_KOOPA_STATE_JUMP_DIE);
				}
				else if (dynamic_cast<CKoopa*>(this->holdingObject)) {
					CKoopa* koopaOther = dynamic_cast<CKoopa*>(this->holdingObject);
					koopaOther->SetIsHolded(false);
					koopaOther->SetState(KOOPA_STATE_JUMP_DIE);
				}
				SetHoldingObject(NULL);
			}
		}
		else if (!isUntouchable)
		{
			if (koopa->GetState() == RED_KOOPA_STATE_SHELL) {
				if (canHold) {
					koopa->SetPosition(x + nx * 24, y);
					koopa->SetIsHolded(true);
					SetHoldingObject(koopa);
				}
				else {
					if (nx > 0) {
						koopa->SetSpeed(RED_KOOPA_SHELL_SCROLL_SPEED, 0);
					}
					else {
						koopa->SetSpeed(-RED_KOOPA_SHELL_SCROLL_SPEED, 0);
					}
					koopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
				}
			}
			else if (koopa->GetState() != RED_KOOPA_STATE_DIE)
			{
				DecreaseLevel();
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	if (koopa->GetIsHolded()) return;

	// jump on top
	if (e->ny < 0 && !this->isOnPlatform)
	{
		if (koopa->GetIsHaveWing()) {
			koopa->SetIsHaveWing(false);
			koopa->SetState(KOOPA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() != KOOPA_STATE_SHELL)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			if (nx > 0) {
				koopa->SetSpeed(KOOPA_SHELL_SCROLL_SPEED, 0);
			}
			else {
				koopa->SetSpeed(-KOOPA_SHELL_SCROLL_SPEED, 0);
			}
			koopa->SetState(KOOPA_STATE_SHELL_SCROLL);
		}
	}
	else // hit by
	{
		if (holdingObject) {
			if (nx > 0 && e->nx < 0 || nx < 0 && e->nx > 0) {
				koopa->SetState(KOOPA_STATE_JUMP_DIE);
				if (dynamic_cast<CRedKoopa*>(this->holdingObject)) {
					CRedKoopa* koopaOther = dynamic_cast<CRedKoopa*>(this->holdingObject);
					koopaOther->SetIsHolded(false);
					koopaOther->SetState(RED_KOOPA_STATE_JUMP_DIE);
				}
				else if (dynamic_cast<CKoopa*>(this->holdingObject)) {
					CKoopa* koopaOther = dynamic_cast<CKoopa*>(this->holdingObject);
					koopaOther->SetIsHolded(false);
					koopaOther->SetState(KOOPA_STATE_JUMP_DIE);
				}
				SetHoldingObject(NULL);
			}
		}
		else if (!isUntouchable)
		{
			if (koopa->GetState() == KOOPA_STATE_SHELL) {
				if (canHold) {
					koopa->SetPosition(x + nx * 24, y);
					koopa->SetIsHolded(true);
					SetHoldingObject(koopa);
				}
				else {
					if (nx > 0) {
						koopa->SetSpeed(KOOPA_SHELL_SCROLL_SPEED, 0);
					}
					else {
						koopa->SetSpeed(-KOOPA_SHELL_SCROLL_SPEED, 0);
					}
					koopa->SetState(KOOPA_STATE_SHELL_SCROLL);
				}
			}
			else if (koopa->GetState() != KOOPA_STATE_DIE)
			{
				DecreaseLevel();
			}
		}
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() != MUSHSHROOM_STATE_EARNED) {
		e->obj->SetState(MUSHSHROOM_STATE_EARNED);
	}
	if (level == MARIO_LEVEL_SMALL) {
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
		level = MARIO_LEVEL_BIG;
	}
}

void CMario::OnCollisionWithGreenMushroom(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() != GREEN_MUSHSHROOM_STATE_EARNED) {
		e->obj->SetState(GREEN_MUSHSHROOM_STATE_EARNED);
	}
}

void CMario::OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() != SUPER_LEAF_STATE_EARNED) {
		e->obj->SetState(SUPER_LEAF_STATE_EARNED);
	}
	if (level == MARIO_LEVEL_SMALL) {
		y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = MARIO_LEVEL_RACCOON;
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->SetState(COIN_STATE_EARNED);
	e->obj->Delete();
}


void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);

	if (e->ny > 0)
	{
		if (questionBlock->GetState() != QUESTION_BLOCK_STATE_OPENED)
		{
			questionBlock->SetState(QUESTION_BLOCK_STATE_OPENED);
		}
	}
}


void CMario::OnCollisionWithDeadline(LPCOLLISIONEVENT e)
{
	CDeadline* deadline = dynamic_cast<CDeadline*>(e->obj);

	dieWithoutJump = true;
	SetState(MARIO_STATE_DIE);
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	SetCollidedPortal(p);
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (e->ny > 0)
	{
		if (brick->GetState() == BRICK_STATE_IDLE && brick->GetItem() != NULL) {
			brick->SetState(BRICK_STATE_TOGGLED);
		}
	}

	if (brick->GetState() == BRICK_STATE_TURNED) {
		brick->SetState(BRICK_STATE_EARNED);
		brick->Delete();
	}
}

void CMario::OnCollisionWithPSwitch(LPCOLLISIONEVENT e)
{
	CPSwitch* pSwitch = dynamic_cast<CPSwitch*>(e->obj);
	if (e->ny < 0)
	{
		if (pSwitch->GetState() == P_SWITCH_STATE_IDLE) {
			pSwitch->SetState(P_SWITCH_STATE_PUSHED);
		}
	}
}

void CMario::OnCollisionWithGoal(LPCOLLISIONEVENT e)
{
	CGoal* goal = dynamic_cast<CGoal*>(e->obj);
	if (e->ny > 0)
	{
		if (goal->GetState() == GOAL_STATE_IDLE) {
			goal->SetState(GOAL_STATE_CLEAR);
			SetState(MARIO_STATE_GOAL);
		}
	}
}

void CMario::RunPortalMovement(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	int height;
	switch (level)
	{
	case MARIO_LEVEL_RACCOON:
		height = MARIO_RACCOON_BBOX_HEIGHT;
		break;
	case MARIO_LEVEL_BIG:
		height = MARIO_BIG_BBOX_HEIGHT;
		break;
	default:
		height = MARIO_SMALL_BBOX_HEIGHT;
		break;
	}

	if (collidedPortal != NULL) {
		if (collidedPortal->GetPortalDirection() == PORTAL_DIRECTION_DOWN) {
			if (y > initial_y + height) {
				y = initial_y + height + MARIO_PORTAL_OFFSET;
			}
		}
		else if (collidedPortal->GetPortalDirection() == PORTAL_DIRECTION_UP) {
			if (y < initial_y - height) {
				y = initial_y - height - MARIO_PORTAL_OFFSET;
			}
		}
	}
	else {
		if (portalOutDirection == PORTAL_DIRECTION_DOWN) {
			if (y > initial_y + height) {
				y = initial_y + height;
			}
		}
		else if (portalOutDirection == PORTAL_DIRECTION_UP) {
			if (y < initial_y - height) {
				y = initial_y - height;
			}
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (state == MARIO_STATE_PORTAL || state == MARIO_STATE_PORTAL_OUT) {
		aniId = ID_ANI_MARIO_SMALL_PORTAL;
	}
	else if (holdingObject) {
		if (vx == 0)
		{
			if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_HOLD_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_IDLE_HOLD_LEFT;
		}
		else if (vx > 0)
		{
			if (ax < 0)
				aniId = ID_ANI_MARIO_SMALL_WALKING_HOLD_LEFT;
			else
				aniId = ID_ANI_MARIO_SMALL_WALKING_HOLD_RIGHT;
		}
		else // vx < 0
		{
			if (ax > 0)
				aniId = ID_ANI_MARIO_SMALL_WALKING_HOLD_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_WALKING_HOLD_LEFT;
		}
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (accelPoint > MARIO_ACCEL_POWER_X / 2)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else 
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (accelPoint > MARIO_ACCEL_POWER_X / 2)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (state == MARIO_STATE_PORTAL || state == MARIO_STATE_PORTAL_OUT) {
		aniId = ID_ANI_MARIO_PORTAL;
	}
	else if (holdingObject) {
		if (vx == 0)
		{
			if (nx > 0) aniId = ID_ANI_MARIO_IDLE_HOLD_RIGHT;
			else aniId = ID_ANI_MARIO_IDLE_HOLD_LEFT;
		}
		else if (vx > 0)
		{
			if (ax < 0)
				aniId = ID_ANI_MARIO_WALKING_HOLD_LEFT;
			else
				aniId = ID_ANI_MARIO_WALKING_HOLD_RIGHT;
		}
		else // vx < 0
		{
			if (ax > 0)
				aniId = ID_ANI_MARIO_WALKING_HOLD_RIGHT;
			else
				aniId = ID_ANI_MARIO_WALKING_HOLD_LEFT;
		}
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (accelPoint > MARIO_ACCEL_POWER_X / 2)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (accelPoint > MARIO_ACCEL_POWER_X / 2)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for raccoon Mario
//
int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (state == MARIO_STATE_PORTAL || state == MARIO_STATE_PORTAL_OUT) {
		aniId = ID_ANI_RACCOON_MARIO_PORTAL;
	}
	else if (holdingObject) {
		if (vx == 0)
		{
			if (nx > 0) aniId = ID_ANI_RACCOON_MARIO_IDLE_HOLD_RIGHT;
			else aniId = ID_ANI_RACCOON_MARIO_IDLE_HOLD_LEFT;
		}
		else if (vx > 0)
		{
			if (ax < 0)
				aniId = ID_ANI_RACCOON_MARIO_WALKING_HOLD_LEFT;
			else
				aniId = ID_ANI_RACCOON_MARIO_WALKING_HOLD_RIGHT;
		}
		else // vx < 0
		{
			if (ax > 0)
				aniId = ID_ANI_RACCOON_MARIO_WALKING_HOLD_RIGHT;
			else
				aniId = ID_ANI_RACCOON_MARIO_WALKING_HOLD_LEFT;
		}
	}
	else if (!isOnPlatform)
	{
		if (isTailJumping) {
			if (nx >= 0)
				if (accelPoint == MARIO_ACCEL_POWER_X) {
					aniId = ID_ANI_RACCOON_MARIO_JUMP_FLY_RIGHT;
				}
				else {
					aniId = ID_ANI_RACCOON_MARIO_JUMP_SLIDE_RIGHT;
				}
			else
				if (accelPoint == MARIO_ACCEL_POWER_X) {
					aniId = ID_ANI_RACCOON_MARIO_JUMP_FLY_LEFT;
				}
				else {
					aniId = ID_ANI_RACCOON_MARIO_JUMP_SLIDE_LEFT;
				}
		}
		else if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_RACCOON_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_RACCOON_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_RACCOON_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_RACCOON_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
		{
			if (isTailAttacking) {
				if (nx > 0) aniId = ID_ANI_RACCOON_MARIO_ATTACK_RIGHT;
				else aniId = ID_ANI_RACCOON_MARIO_ATTACK_LEFT;
			}
			else if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_RACCOON_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_RACCOON_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_RACCOON_MARIO_BRACE_RIGHT;
				else if (accelPoint > MARIO_ACCEL_POWER_X / 2)
					aniId = ID_ANI_RACCOON_MARIO_RUNNING_RIGHT;
				else 
					aniId = ID_ANI_RACCOON_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_RACCOON_MARIO_BRACE_LEFT;
				else if (accelPoint > MARIO_ACCEL_POWER_X / 2)
					aniId = ID_ANI_RACCOON_MARIO_RUNNING_LEFT;
				else 
					aniId = ID_ANI_RACCOON_MARIO_WALKING_LEFT;
			}
		}

	if (aniId == -1) aniId = ID_ANI_RACCOON_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	float alpha = 1.0f;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();

	if (isUntouchable) {
		if (!isFlickering) {
			alpha = alpha / 2;
		}
	}

	animations->Get(aniId)->Render(x, y, false, alpha);
	// RenderBoundingBox();
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE || this->state == MARIO_STATE_GOAL) return;

	if (this->state == MARIO_STATE_PORTAL || this->state == MARIO_STATE_PORTAL_OUT) return;

	switch (state)
	{
	case MARIO_STATE_PORTAL_OUT:
		vx = 0;
		ax = 0;
		initial_y = y;
		renderOrder = -1;
		if (portalOutDirection == PORTAL_DIRECTION_DOWN) {
			vy = MARIO_PORTAL_SPEED;
		}
		else if (portalOutDirection == PORTAL_DIRECTION_UP) {
			vy = -MARIO_PORTAL_SPEED;
		}
		StartPortalOut();
		break;
	case MARIO_STATE_PORTAL:
		vx = 0;
		ax = 0;
		initial_y = y;
		renderOrder = -1;
		float portalX, portalY;
		collidedPortal->GetPosition(portalX, portalY);
		x = portalX;
		if (collidedPortal->GetPortalDirection() == PORTAL_DIRECTION_DOWN) {
			vy = MARIO_PORTAL_SPEED;
		}
		else if (collidedPortal->GetPortalDirection() == PORTAL_DIRECTION_UP) {
			vy = -MARIO_PORTAL_SPEED;
		}
		StartPortal();
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		if (accelPoint == 0) {
			accelPoint = 1;
		}
		ax = accelPoint * ((MARIO_ACCEL_RUN_X)/ MARIO_ACCEL_POWER_X);
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		if (accelPoint == 0) {
			accelPoint = 1;
		}
		ax = -accelPoint * (MARIO_ACCEL_RUN_X/MARIO_ACCEL_POWER_X);
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
			
		} else if (level == MARIO_LEVEL_RACCOON && !holdingObject) {
			if (!isTailJumping) {
				if (accelPoint == MARIO_ACCEL_POWER_X) {
					ay = 0.0f;
					vy = -MARIO_JUMP_TAIL_SPEED_Y;
				}
				else {
					vy = 0.0f;
					ay = MARIO_TAIL_GRAVITY;
				}
				isTailJumping = true;
				tail_jump_start = GetTickCount64();
			}
		}
		break;
	case MARIO_STATE_RELEASE_JUMP: 
		if (isTailJumping) break;
		ay = MARIO_GRAVITY;
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;
	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += AdjustHeight();
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= AdjustHeight();
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_TAIL_ATTACK:
		if (isSitting) break;
		isTailAttacking = true;
		nAttacking = nx;
		tail_attack_start = GetTickCount64();

		tail->SetPosition(x + nx * MARIO_TAIL_OFFSET_X, y + MARIO_TAIL_OFFSET_Y);
		tail->SetDirection(nx, 0);
		tail->SetState(MARIO_TAIL_STATE_ATTACK);
		break;
	case MARIO_STATE_TAIL_ATTACK_RELEASE:
		if (isTailAttacking) {
			isTailAttacking = false;
			tail_attack_start = 0;
			vx = 0;
			tail->SetState(MARIO_TAIL_STATE_IDLE);
		}
		break;
	case MARIO_STATE_DIE:
		if (!dieWithoutJump) {
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		vx = 0;
		ax = 0;
		die_start = GetTickCount64();
		break;
	case MARIO_STATE_GOAL:
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else if (holdingObject) {
			float objL, objT, objR, objB;
			holdingObject->GetBoundingBox(objL, objT, objR, objB);
			float w = objR - objL;
			float h = objB - objT;

			if (nx < 0) {
				left = x - MARIO_BIG_BBOX_WIDTH / 2 - w;
				top = y - MARIO_BIG_BBOX_HEIGHT / 2;
				right = left + MARIO_BIG_BBOX_WIDTH + w;
				bottom = top + MARIO_BIG_BBOX_HEIGHT;
			}
			else {
				left = x - MARIO_BIG_BBOX_WIDTH / 2;
				top = y - MARIO_BIG_BBOX_HEIGHT / 2;
				right = left + MARIO_BIG_BBOX_WIDTH + w;
				bottom = top + MARIO_BIG_BBOX_HEIGHT;
			}
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
		}
		else if (holdingObject) {
			float objL, objT, objR, objB;
			holdingObject->GetBoundingBox(objL, objT, objR, objB);
			float w = objR - objL;
			float h = objB - objT;

			if (nx < 0) {
				left = x - MARIO_RACCOON_BBOX_WIDTH / 2 - w;
				top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
				right = left + MARIO_RACCOON_BBOX_WIDTH + w;
				bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
			}
			else {
				left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
				top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
				right = left + MARIO_RACCOON_BBOX_WIDTH + w;
				bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
			}
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
	else
	{
		if (holdingObject) {
			float objL, objT, objR, objB;
			holdingObject->GetBoundingBox(objL, objT, objR, objB);
			float w = objR - objL;
			float h = objB - objT;

			if (nx < 0) {
				left = x - MARIO_SMALL_BBOX_WIDTH / 2 - w;
				top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
				right = left + MARIO_SMALL_BBOX_WIDTH + w;
				bottom = top + MARIO_SMALL_BBOX_HEIGHT;
			}
			else {
				left = x - MARIO_SMALL_BBOX_WIDTH / 2;
				top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
				right = left + MARIO_SMALL_BBOX_WIDTH + w;
				bottom = top + MARIO_SMALL_BBOX_HEIGHT;
			}
		}
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::DecreaseLevel()
{
	switch (level)
	{
	case MARIO_LEVEL_RACCOON: 
		level = MARIO_LEVEL_BIG;
		StartUntouchable();
		break;
	case MARIO_LEVEL_BIG:
		level = MARIO_LEVEL_SMALL;
		StartUntouchable();
		break;
	case MARIO_LEVEL_SMALL: 
		SetState(MARIO_STATE_DIE);
		break;
	default:
		break;
	}
}

int CMario::AdjustHeight()
{
	int adjustHeight = MARIO_SMALL_BBOX_HEIGHT;
	switch (level)
	{
	case MARIO_LEVEL_RACCOON:
		adjustHeight = (MARIO_RACCOON_BBOX_HEIGHT - MARIO_RACCOON_SITTING_BBOX_HEIGHT) / 2;
		break;
	case MARIO_LEVEL_BIG:
		adjustHeight = (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2;
		break;
	case MARIO_LEVEL_SMALL:
		break;
	default:
		break;
	}
	return adjustHeight;
}

void CMario::SetHoldingObject(CGameObject* holdingObject)
{
	if (holdingObject == NULL) {
		if (dynamic_cast<CRedKoopa*>(this->holdingObject)) {
			CRedKoopa* koopa = dynamic_cast<CRedKoopa*>(this->holdingObject);
			if (koopa->GetState() == RED_KOOPA_STATE_SHELL) {
				if (nx > 0) {
					koopa->SetSpeed(RED_KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				else {
					koopa->SetSpeed(-RED_KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				koopa->SetIsHolded(false);
				koopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
			}
			else if (koopa->GetState() == RED_KOOPA_STATE_RESPAWN) {
				if (nx > 0) {
					koopa->SetSpeed(RED_KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				else {
					koopa->SetSpeed(-RED_KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				koopa->SetIsHolded(false);
				koopa->SetState(RED_KOOPA_STATE_SHELL_SCROLL);
			}
		}
		else if (dynamic_cast<CKoopa*>(this->holdingObject)) {
			CKoopa* koopa = dynamic_cast<CKoopa*>(this->holdingObject);
			if (koopa->GetState() == KOOPA_STATE_SHELL) {
				if (nx > 0) {
					koopa->SetSpeed(KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				else {
					koopa->SetSpeed(-KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				koopa->SetIsHolded(false);
				koopa->SetState(KOOPA_STATE_SHELL_SCROLL);
			} else if (koopa->GetState() == KOOPA_STATE_RESPAWN) {
				if (nx > 0) {
					koopa->SetSpeed(KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				else {
					koopa->SetSpeed(-KOOPA_SHELL_SCROLL_SPEED, 0);
				}
				koopa->SetIsHolded(false);
				koopa->SetState(KOOPA_STATE_SHELL_SCROLL);
			}
		}
	};

	this->holdingObject = holdingObject;
}