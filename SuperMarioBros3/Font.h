#pragma once
#include <string>

using namespace std;

#define FONT_ID_0 40001
#define FONT_ID_1 40002
#define FONT_ID_2 40003
#define FONT_ID_3 40004
#define FONT_ID_4 40005
#define FONT_ID_5 40006
#define FONT_ID_6 40007
#define FONT_ID_7 40008
#define FONT_ID_8 40009
#define FONT_ID_9 40010

#define FONT_SPACING 8

#define FONT_PLAY_CHARACTER 40047
#define FONT_ARROW_WHITE 40049
#define FONT_P_WHITE 40050
#define FONT_ARROW_BLACK 40051
#define FONT_P_BLACK 40052

class CFont
{
private:
	int max;
public:

	void DrawNumber(int max, float x, float y, int s = 0);
	void DrawCharacter(int id, float x, float y);
	int convert(char c);
};