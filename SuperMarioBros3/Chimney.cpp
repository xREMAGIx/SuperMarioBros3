#include "Chimney.h"

CChimney::CChimney(float x, float y, int length, int height, int _id_item,  vector<LPGAMEOBJECT>* objects) :CGameObject(x, y)
{
	this->length = length;
	this->height = height;
	dt_reset = -1;
	object = NULL;
	switch (_id_item)
	{
	case OBJECT_TYPE_VENUS_FIRE_TRAP:
		object = new CVenusFireTrap(x + 8, y - VENUS_FIRE_TRAP_HEIGHT / 2 - 6);
		objects->push_back(object);
		break;
	}

	//General setup
	SetState(CHIMNEY_STATE_IDLE);
}

void CChimney::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;
	float xy = y - this->cellHeight / 2 + rect.bottom / 2;


	CGame::GetInstance()->Draw(xx - cx, xy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CChimney::Render()
{
	if (this->length <= 0) return;

	RenderBoundingBox();
}

void CChimney::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight * this->height;
}

void CChimney::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (object != NULL && state != CHIMNEY_STATE_CANCEL) {
		//object->Update(dt, coObjects);
	}
	else {
		if (dt_reset != 0 && GetTickCount64() - dt_reset > TIME_CHIMNEY_RESET) {
			SetState(CHIMNEY_STATE_IDLE);
			StopReset();
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CChimney::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == CHIMNEY_STATE_CANCEL) {
		StartReset();
	}
}