#include "Chimney.h"

CChimney::CChimney(int width, int height, int _id_item, int ani_set_id, int x, int y, vector<LPGAMEOBJECT>* objects)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	this->width = width;
	this->height = height;
	switch (_id_item)
	{
	case OBJECT_TYPE_VENUS_FIRE_TRAP:
		object = new CVenusFireTrap();
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		object->SetAnimationSet(ani_set);
		object->SetId(_id_item);
		dynamic_cast<CVenusFireTrap*>(object)->SetPosition(x + 8, y - VENUS_FIRE_TRAP_HEIGHT);
		objects->push_back(object);
		break;
	}

	//General setup
}


void CChimney::Render()
{
	/*
	if (object != NULL && state != CHIMNEY_STATE_CANCEL) {
		object->Render();
	}
	*/
}

void CChimney::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (object != NULL && state != CHIMNEY_STATE_CANCEL) {
		//object->Update(dt, coObjects);
	}
	else {
		if (dt_reset != 0 && GetTickCount() - dt_reset > TIME_CHIMNEY_RESET) {
			SetState(CHIMNEY_STATE_IDLE);
			StopReset();
		}
	}

	//SetState(CHIMNEY_STATE_IDLE);
}


void CChimney::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void CChimney::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == CHIMNEY_STATE_CANCEL) {
		StartReset();
	}
}
