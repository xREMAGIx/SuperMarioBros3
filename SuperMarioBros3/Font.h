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

class CFont
{
private:
	int max;
public:

	void DrawNumber(int max, float x, float y, int s = 0);
	int convert(char c);
};