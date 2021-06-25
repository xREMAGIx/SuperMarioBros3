#pragma once
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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define ID_TEX_MAP1 11
#define TILE_SIZE 16
#define BRICK_SIZE 16

using namespace std;

class GameMap
{
public:
	GameMap();
	~GameMap();

	int tilesetId;

	int listTile[200][200];

	int numTilesCols, spacing;
	int numRows, numCols;
	int id, cellW, cellH;
	int currentRow;

	RECT GetSourceRect(int index);
	void LoadMap(LPCWSTR filepath);
	void Render();
	int getRows() { return numRows; }
	int getCols() { return numCols; }
	int getTileW() { return cellW; }
	int getTileH() { return cellH; }

	void _ParseSection_READDATA(string line);
	void _ParseSection_MAP(string line);

	static GameMap* GetInstance();

private:
	static GameMap* __instance;
	int getTitle(int x, int y);
};

