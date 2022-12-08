#include "PlayerFont.h"

CPlayerFont::CPlayerFont(float x, float y) :CGameObject(x, y)
{
	sceneId = 1;
}

void CPlayerFont::Render()
{
	int spriteId = ID_SPRITE_PLAYER_FONT_1;

	switch (state)
	{
	case PLAYER_FONT_STATE_ONE:
		spriteId = ID_SPRITE_PLAYER_FONT_1;
		break;
	case PLAYER_FONT_STATE_TWO:
		spriteId = ID_SPRITE_PLAYER_FONT_2;
		break;
	default:
		break;
	}

	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(spriteId)->Draw(x, y);
	//RenderBoundingBox();
}

void CPlayerFont::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CPlayerFont::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
