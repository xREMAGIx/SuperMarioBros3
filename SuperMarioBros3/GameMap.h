#pragma once
#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Textures.h"
#include "Game.h"
#include "Sprites.h"
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "Animation.h"
#include "Utils.h"

using namespace std;

#define TILE_SIZE 16
#define BRICK_SIZE 16

class CGameMap
{
protected:
	static CGameMap* __instance;
	int tilesetId;

	int listTile[200][200];

	int numTilesCols, spacing;
	int numRows, numCols;
	int id, cellW, cellH;
	int currentRow;

public:
	CGameMap();

	RECT GetSourceRect(int index);
	void LoadMap(LPCWSTR filepath);
	void Render();
	int getTilesetId() { return tilesetId; }
	int getRows() { return numRows; }
	int getCols() { return numCols; }
	int getTileW() { return cellW; }
	int getTileH() { return cellH; }
	
	void _ParseSection_READDATA(string line);
	void _ParseSection_MAP(string line);

	int getTitle(int x, int y);

	static CGameMap* GetInstance();
	~CGameMap();

};

typedef CGameMap* LPGAMEMAP;