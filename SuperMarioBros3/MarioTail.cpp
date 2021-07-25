#include "Game.h"
#include "MarioTail.h"


CMarioTail::CMarioTail()
{
}

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx < 0) {
		left = x + MARIO_TAIL_OFFSET_WIDTH * 2 - MARIO_TAIL_BBOX_WIDTH;
		top = y + MARIO_TAIL_OFFSET_HEIGHT;
		right = x + MARIO_TAIL_OFFSET_WIDTH * 2;
		bottom = y + MARIO_TAIL_OFFSET_HEIGHT + MARIO_TAIL_BBOX_HEIGHT;
	}
	else {
		left = x + MARIO_TAIL_OFFSET_WIDTH;
		top = y + MARIO_TAIL_OFFSET_HEIGHT;
		right = x + MARIO_TAIL_OFFSET_WIDTH + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_OFFSET_HEIGHT + MARIO_TAIL_BBOX_HEIGHT;
	}
	
}

void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++) {

		LPGAMEOBJECT e = coObjects->at(i);

		if (dynamic_cast<CGoomba*>(e))
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(e);
			if (this->IsCollisedWith(goomba))
			{
				goomba->SetUpSideDown(1);
				goomba->SetState(GOOMBA_STATE_JUMP_DIE);
			}
		}

		if (dynamic_cast<CBrick*>(e))
		{
			CBrick* brick = dynamic_cast<CBrick*>(e);
			if (this->IsCollisedWith(brick))
			{
				brick->SetState(BRICK_STATE_BREAK);
			}			
		}
	}
}

void CMarioTail::Render()
{
	/*
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20022);
	sprite->Draw(x + MARIO_TAIL_OFFSET_WIDTH, y + MARIO_TAIL_OFFSET_HEIGHT);
	*/
}

void CMarioTail::SetState(int state)
{
	CGameObject::SetState(state);
}
