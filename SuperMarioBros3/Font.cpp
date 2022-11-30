#include "Font.h"

void CFont::DrawNumber(int max, float x, float y, int s)
{

	if (s < 0)
		s = 0;

	CSprites* sprites = CSprites::GetInstance();
	string str = to_string(s);

	for (int i = 0; i < max - str.size(); i++)
	{
		sprites->Get(ID_SPRITE_FONT_0)->Draw(x, y);
		x += FONT_SPACING;
	}
	for (int i = 0; i < str.size(); i++)
	{
		sprites->Get(ID_SPRITE_FONT_0 + convert(str[i]))->Draw(x, y);
		x += FONT_SPACING;
	}

}

void CFont::DrawCharacter(int id, float x, float y)
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(id)->Draw(x, y);
}

int CFont::convert(char c)
{
	return c - '0';
}