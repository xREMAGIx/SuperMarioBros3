#pragma once

#include <fstream>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>
#include <fstream>
#include <string>
#include "Sprites.h"
#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

//CONSTANTS
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Super Mario Bros 3"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_FRAME_RATE 60

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_HUD 30
#define ID_TEX_INTRO 40

//END-CONSTANTS

using namespace std;
void LoadSprites(LPCWSTR line, LPDIRECT3DTEXTURE9 texture);
void LoadAnimations(LPCWSTR line);

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);


int GetDistanceX(RECT r1, RECT r2);
int GetDistanceY(RECT r1, RECT r2);
int GetRectW(RECT r);
int GetRectH(RECT r);

class Utils
{
public:
	static bool IsOverlapX(RECT r1, RECT r2)
	{
		return !(r1.right < r2.left || r1.left > r2.right);
	}

	static bool IsOverlapY(RECT r1, RECT r2)
	{
		return !(r1.top > r2.bottom || r1.bottom < r2.top);
	}

};
