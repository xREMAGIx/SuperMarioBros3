#include "Game.h"
#include "SuperLeaf.h"

CSuperLeaf::CSuperLeaf(float x, float y) :CGameObject(x, y)
{
	this->vx = SUPER_LEAF_SPEED_X;
	this->vy = SUPER_LEAF_SPEED_Y;
};

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SUPER_LEAF_BBOX_WIDTH;
	bottom = y + SUPER_LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CSuperLeaf::Render()
{
	LPSPRITE sprite;
	sprite = CSprites::GetInstance()->Get(20002);
	if (vx < 0) {
		sprite->DrawFlipX(x, y);
	}
	else {
		sprite->Draw(x, y);
	}
}

void CSuperLeaf::SetState(int state)
{

}
