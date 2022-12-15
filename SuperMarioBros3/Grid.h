#pragma once
#include <vector>
#include "GameObject.h"
#include "Game.h"

#define GRID_CELL_HEIGHT SCREEN_HEIGHT
#define GRID_CELL_WIDTH SCREEN_WIDTH

#define GRID_CELL_OFFSET_X 80
#define GRID_CELL_OFFSET_Y 80

using namespace std;

class Grid
{
	static Grid* __instance;

	vector<LPGAMEOBJECT>** cells;
public:
	static bool IsOutOfCamera(float cam_x, float cam_y, LPGAMEOBJECT obj);
	static Grid* GetInstance();

	int Row = 100;
	int Col = 50;
	int CellW = 16;
	int CellH = 16;
	void LoadObject(string file_path);
	void Insert(int id, int grid_x, int grid_y, int type, int trend, float x, float y, float w, float h, int id_item, int object);
	void GetListObject(vector<LPGAMEOBJECT>& NewObj, vector<LPGAMEOBJECT>& ListObj, float cam_x, float cam_y);
};

