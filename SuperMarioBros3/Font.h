#pragma once
#include <string>

#include "Sprites.h"
#include "AssetIDs.h"

using namespace std;

#define FONT_SPACING 8
#define FONT_TEXT_SPACING 10
#define FONT_HEIGHT 8
#define SPACE_ASCII_CHAR 32

class CFont
{
private:
	int max;
public:

	void DrawNumber(int max, float x, float y, int s = 0);
	void DrawCharacter(int id, float x, float y);
	void DrawTextString(string s, float x, float y);
	int convert(char c);
	int ConvertText(char c);
};