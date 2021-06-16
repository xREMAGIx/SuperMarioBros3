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
#define ID_TEX_MAP1 1
#define TILE_SIZE 16
#define BRICK_SIZE 16

using namespace std;


class GameMap
{
public:
	GameMap();
	~GameMap();

	int scene;

	int width;
	int height;
	int col;
	int row;
	int listTile[200][200];

	int numTilesCols, spacing;
	int numRows, numCols;
	int id, cellW, cellH;

	RECT GetSourceRect(int index);
	void LoadMap(const char* filepath);
	void DrawMap();
	int getRows() { return numRows; }
	int getCols() { return numCols; }
	int getTileW() { return cellW; }
	int getTileH() { return cellH; }

private:
	int getTitle(int x, int y);
};

