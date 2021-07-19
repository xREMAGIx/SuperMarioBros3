#include "Grid.h"
#include "GameMap.h"

Grid* Grid::__instance = NULL;

Grid* Grid::GetInstance()
{
	if (__instance == NULL)
		__instance = new Grid();
	return __instance;
}

void Grid::LoadObject(string file_path)
{
}

void Grid::Insert(int id, int grid_x, int grid_y, int type, int trend, float x, float y, float w, float h, int id_item, int object)
{
}


bool Grid::IsOutOfCamera(float cam_x, float cam_y, LPGAMEOBJECT obj)
{
	RECT rectThis, rect1;

	CGame* game = CGame::GetInstance();

	// CAM
	float l = cam_x -80,
		t = cam_y,
		r = cam_x + game->GetScreenWidth() + 160,
		b = cam_y + game->GetScreenHeight() + 160;

	rectThis.left = (int)l;
	rectThis.top = (int)t;
	rectThis.right = (int)r;
	rectThis.bottom = (int)b;

	//obj
	float l1, t1, r1, b1;
	obj->GetBoundingBox(l1, t1, r1, b1);
	rect1.left = (int)l1;
	rect1.top = (int)t1;
	rect1.right = (int)r1;
	rect1.bottom = (int)b1;

	return !CGame::GetInstance()->isCollision(rectThis, rect1);
}

void Grid::GetListObject(vector<LPGAMEOBJECT>& NewObj, vector<LPGAMEOBJECT>& ListObj, float cam_x, float cam_y)
{
	vector<LPGAMEOBJECT> mapObject;

	for (int i = 0; i < ListObj.size(); i++) {
		if (!Grid::IsOutOfCamera(cam_x, cam_y, ListObj.at(i)))
		{
			mapObject.push_back(ListObj.at(i));
		}
	}

	NewObj.clear();

	for (auto& x : mapObject)
	{
		NewObj.push_back(x);
	}
}
