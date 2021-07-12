#include "Game.h"
#include "PlayerFont.h"

void CPlayerFont::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CPlayerFont::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CPlayerFont::Render()
{
	LPSPRITE sprite;
	
	sprite = CSprites::GetInstance()->Get(PLAYER_FONT_SPRITE_ONE);

	if(state == PLAYER_FONT_STATE_TWO)
		sprite = CSprites::GetInstance()->Get(PLAYER_FONT_SPRITE_TWO);

	sprite->Draw(x, y);
}

void CPlayerFont::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLAYER_FONT_STATE_ONE:
		break;
	case PLAYER_FONT_STATE_TWO:
		break;
	}
}
