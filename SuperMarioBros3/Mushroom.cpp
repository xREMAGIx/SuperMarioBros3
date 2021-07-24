#include "Mushroom.h"

CMushroom::CMushroom()
{
	SetState(MUSHSHROOM_STATE_WALKING);
	score = new CPoint();
	score->SetPointId(POINT_ID_1000);
}

void CMushroom::Render()
{
	if (state != MUSHSHROOM_STATE_EARNED) {
		LPSPRITE sprite;
		sprite = CSprites::GetInstance()->Get(20001);
		sprite->Draw(x, y);
	}
	else {
		score->Render();
	}
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != MUSHSHROOM_STATE_EARNED)
	{
		l = x;
		t = y;
		r = x + MUSHSHROOM_BBOX_WIDTH;
		b = y + MUSHSHROOM_BBOX_HEIGHT;
	}
	else {

	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vy += MUSHSHROOM_GRAVITY * dt;
	CalcPotentialCollisions(coObjects, coEvents);

	if (state == MUSHSHROOM_STATE_EARNED) {
		score->Update(dt, coObjects);
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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		
		if (ny != 0) vy = 0;

		// Collision logic with world
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Walk on invisible block
			if (dynamic_cast<CInvisibleBlock*>(e->obj)) // if e->obj is Block 
			{
				if (ny != 0) vy = 0;
			}

			//Touch question block
			if (dynamic_cast<CQuestionBlock*>(e->obj)) // if e->obj is Block 
			{
				if (nx != 0) {
					this->vx = -vx;
					this->nx = -nx;
				}
				
			}

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				if (nx != 0) {
					this->vx = -vx;
					this->nx = -nx;
				};
			}

			if (dynamic_cast<CMario*>(e->obj)) // if e->obj is Block 
			{
				LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
				CMario* mario = ((CPlayScene*)scene)->GetPlayer();
				if (state != MUSHSHROOM_STATE_SHOWING) // if e->obj is Block 
				{
					if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
						mario->SetLevel(MARIO_LEVEL_BIG);
					}
					SetState(MUSHSHROOM_STATE_EARNED);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case MUSHSHROOM_STATE_SHOWING: {
			vy = -MUSHSHROOM_SPEED;
			break;
		}
		case MUSHSHROOM_STATE_WALKING: {
			vx = nx*MUSHSHROOM_SPEED;
			break;
		}
		case MUSHSHROOM_STATE_EARNED: {
			score->SetPosition(x, y - 18);
			score->SetState(POINT_STATE_SHOW);
			CBoard* game_board = CBoard::GetInstance();
			game_board->AddPoint(1000);
			break;
		}
	}
}
