#include "Font.h"
#include "Sprites.h"

void CFont::DrawNumber(int max, float x, float y, int s)
{
	/*
	if (s < 0)
		s = 0;

	CSprites* sprites = CSprites::GetInstance();
	string str = to_string(s);

	for (int i = 0; i < max - str.size(); i++)
	{
		sprites->Get(60000)->Draw(x, y);
		x += 15;
	}
	for (int i = 0; i < str.size(); i++)
	{
		sprites->Get(60000 + convert(str[i]))->Draw(x, y);
		x += 15;
	}
	*/
}

int CFont::convert(char c)
{
	return c - '0';
}