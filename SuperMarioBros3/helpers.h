#pragma once

#include <fstream>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>
#include <fstream>
#include <string>
#include "Sprites.h"

void LoadSprites(CSprites* sprites, const char* fileDir, LPDIRECT3DTEXTURE9 texture);
void LoadAnimations(CAnimations* animations, const char* fileDir);
