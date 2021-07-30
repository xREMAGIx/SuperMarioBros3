#include "GreenMushroom.h"

CGreenMushroom::CGreenMushroom()
{
	SetState(GREEN_MUSHSHROOM_STATE_WALKING);
	score = new CPoint();
	score->SetPointId(POINT_ID_LEVELUP);
}

void CGreenMushroom::Render()
{
	if (state != GREEN_MUSHSHROOM_STATE_EARNED) {
		LPSPRITE sprite;
		sprite = CSprites::GetInstance()->Get(20006);
		sprite->Draw(x, y);
	}
	else {
		score->Render();
	}
}

void CGreenMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != GREEN_MUSHSHROOM_STATE_EARNED)
	{
		l = x;
		t = y;
		r = x + GREEN_MUSHSHROOM_BBOX_WIDTH;
		b = y + GREEN_MUSHSHROOM_BBOX_HEIGHT;
	}
	else {

	}
}

void CGreenMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vy += GREEN_MUSHSHROOM_GRAVITY * dt;
	CalcPotentialCollisions(coObjects, coEvents);

	if (state == GREEN_MUSHSHROOM_STATE_EARNED) {
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
					this->nx = -this->nx;
				}

			}

			if (dynamic_cast<CChimney*>(e->obj)) // if e->obj is Block 
			{
				if (nx != 0) {
					this->vx = -vx;
					this->nx = -this->nx;
				};
			}

			if (dynamic_cast<CMario*>(e->obj)) // if e->obj is Block 
			{
				LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
				CMario* mario = ((CPlayScene*)scene)->GetPlayer();
				if (state != GREEN_MUSHSHROOM_STATE_SHOWING) // if e->obj is Block 
				{
					SetState(GREEN_MUSHSHROOM_STATE_EARNED);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGreenMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_MUSHSHROOM_STATE_SHOWING: {
		vy = -GREEN_MUSHSHROOM_SPEED;
		break;
	}
	case GREEN_MUSHSHROOM_STATE_WALKING: {
		vx = nx * GREEN_MUSHSHROOM_SPEED;
		break;
	}
	case GREEN_MUSHSHROOM_STATE_EARNED: {
		score->SetPosition(x, y - 18);
		score->SetState(POINT_STATE_SHOW);
		CBoard* game_board = CBoard::GetInstance();
		game_board->AddLives();
		break;
	}
	}
}
