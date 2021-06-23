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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

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
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						SetState(MARIO_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
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
				if (ny != 0) vy = 0;
				if (nx != 0) vx = 0;
				CCloud* block = dynamic_cast<CCloud*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					SetState(MARIO_STATE_IDLE);
				}
			}



			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				CInvisibleBlock* block = dynamic_cast<CInvisibleBlock*>(e->obj);
				if (e->ny < 0 && (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_RIGHT || state == MARIO_STATE_JUMP_LEFT))
				{
					if (ny != 0) vy = 0;
					SetState(MARIO_STATE_IDLE);
				}

				if (e->ny > 0)
				{
					x += dx;
					y += dy;
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
		if (y == 100)
			vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		/*
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		*/
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}