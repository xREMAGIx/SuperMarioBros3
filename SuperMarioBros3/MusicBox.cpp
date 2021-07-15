#include "Game.h"
#include "MusicBox.h"

CMusicBox::CMusicBox()
{
	SetState(MUSIC_BOX_STATE_IDLE);
}

void CMusicBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MUSIC_BOX_BBOX_WIDTH;
	bottom = y + MUSIC_BOX_BBOX_HEIGHT;
}

void CMusicBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CMusicBox::Render()
{
	if (state == MUSIC_BOX_STATE_HIDDEN) {
		int ani = MUSIC_BOX_ANI_SPECIAL;
		animation_set->at(ani)->Render(x, y, -1, 0);
	}
	else {

		int ani = MUSIC_BOX_ANI_IDLE;
		animation_set->at(ani)->Render(x, y, -1, 255);
	}
}

void CMusicBox::SetState(int state)
{
	CGameObject::SetState(state);
}
