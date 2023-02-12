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

void CFont::DrawTextString(string str, float x, float y)
{
	float initX = x - (str.size() / 2) * FONT_TEXT_SPACING;
	CSprites* sprites = CSprites::GetInstance();

	for (int i = 0; i < str.size(); i++)
	{
		int id = ConvertText(str[i]);

		if (id != -1) {
			sprites->Get(ID_SPRITE_FONT_BASE + id)->Draw(initX, y);
		}
		initX += FONT_TEXT_SPACING;

	}
}

int CFont::convert(char c)
{
	return c - '0';
}

int CFont::ConvertText(char c)
{
	int asciiChar = int(c);
	if (asciiChar == SPACE_ASCII_CHAR)
	{ 
		return -1;
	}	
	return asciiChar;
}