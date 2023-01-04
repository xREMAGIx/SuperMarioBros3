#include "Wing.h"
#include "PlayScene.h"

CWing::CWing(float x, float y) :CGameObject(x, y)
{
	state = WING_STATE_IDLE;
	nx = -1;
}

void CWing::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WING_BBOX_WIDTH / 2;
	top = y - WING_BBOX_HEIGHT / 2;
	right = left + WING_BBOX_WIDTH;
	bottom = top + WING_BBOX_HEIGHT;
}

void CWing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == WING_STATE_DESTROY)
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
}

void CWing::Render()
{
	int aniId = ID_ANI_WING_LEFT;

	switch (state)
	{
		case WING_STATE_IDLE: 
			if (nx > 0) aniId = ID_ANI_WING_RIGHT;
			else aniId = ID_ANI_WING_LEFT;
			break;
		case WING_STATE_FLAP:
			if (nx > 0) aniId = ID_ANI_WING_FLAP_RIGHT;
			else aniId = ID_ANI_WING_FLAP_LEFT;
			break;
		default:
			break;
	}

	if (state != WING_STATE_DESTROY)
	{
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
		RenderBoundingBox();
	}
}

void CWing::SetState(int state)
{
	switch (state)
	{
	case WING_STATE_IDLE: {
		break;
	}
	default:
		break;
	}
	CGameObject::SetState(state);
}
