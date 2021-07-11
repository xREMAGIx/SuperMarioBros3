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

#define BACKGROUND_COLOR D3DCOLOR_XRGB(156, 252, 240)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_FRAME_RATE 60

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_HUD 30
#define ID_TEX_INTRO 40

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS 1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BLOCK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_QUESTION_BLOCK	3
#define OBJECT_TYPE_INVISIBLE_BLOCK 4
#define OBJECT_TYPE_BRICK 5
#define OBJECT_TYPE_COIN 6
#define OBJECT_TYPE_CLOUD 7
#define OBJECT_TYPE_GREEN_KOOPA 8
#define OBJECT_TYPE_RED_KOOPA 9
#define OBJECT_TYPE_PARA_GOOMBA 10
#define OBJECT_TYPE_PARA_KOOPA 11
#define OBJECT_TYPE_INVISIBLE_WALL 12
#define OBJECT_TYPE_CHIMNEY 13
#define OBJECT_TYPE_ENEMY_WALL 14
#define OBJECT_TYPE_MUSHROOM 15


#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_TREE_WORLD 100
#define OBJECT_TYPE_MARIO_WORLD 101
#define OBJECT_TYPE_MAP_POINT 102


#define MAX_SCENE_LINE 1024


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
