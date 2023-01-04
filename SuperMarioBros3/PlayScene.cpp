#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"

//HUD
#include "Platform.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	marioWorld = NULL;
	gameBoard = NULL;
	map = NULL;
	choosePlayer = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAPFILE	3
#define SCENE_SECTION_SETTINGS	4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 12) return; // skip invalid lines

	int scene_width = atoi(tokens[0].c_str());
	int sceen_height = atoi(tokens[1].c_str());
	int cam_x = atoi(tokens[2].c_str());
	int cam_y = atoi(tokens[3].c_str());
	int max_cam_x = atoi(tokens[4].c_str());
	int max_cam_y = atoi(tokens[5].c_str());
	int min_cam_x = atoi(tokens[6].c_str());
	int min_cam_y = atoi(tokens[7].c_str());
	int bg_r = atoi(tokens[8].c_str());
	int bg_g = atoi(tokens[9].c_str());
	int bg_b = atoi(tokens[10].c_str());
	int previous_scene = atoi(tokens[11].c_str());

	CGame* game = CGame::GetInstance();
	game->SetMaxCamScreen(max_cam_x, max_cam_y);
	game->SetMinCamScreen(min_cam_x, min_cam_y);
	game->SetScreenSize(scene_width, sceen_height);
	game->SetCamPos(cam_x, cam_y);
	game->SetBackgroundColor(D3DXCOLOR(bg_r * 1.0f/255, bg_g * 1.0f/255, bg_b * 1.0f/255, 1.0f));
	game->SetPreviousSceneId(previous_scene);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_MARIO_WORLD:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO WORLD was created before!\n");
			return;
		}
		obj = new CMarioWorld();
		obj->SetPosition(x, y);
		marioWorld = (CMarioWorld*)obj;

		DebugOut(L"[INFO] MARIO WORLD object created!\n");
		break;

	//ENEMIES
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_RED_GOOMBA: obj = new CRedGoomba(x, y, true); break;
	case OBJECT_TYPE_KOOPA: obj = new CKoopa(x, y); break;
	case OBJECT_TYPE_RED_KOOPA: obj = new CRedKoopa(x, y); break;

	// MISC
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new CMushroom(x, y); break;
	case OBJECT_TYPE_GREEN_MUSHROOM: obj = new CGreenMushroom(x, y); break;
	case OBJECT_TYPE_QUESTION_BLOCK:
	{
		int item_id = atoi(tokens[3].c_str());
		obj = new CQuestionBlock(x, y, item_id); break; 
	}
	case OBJECT_TYPE_TREE_WORLD: obj = new CTreeWorld(x, y); break;
	case OBJECT_TYPE_CLOUD: obj = new CCloud(x, y); break;
	case OBJECT_TYPE_BLOCK: obj = new CBlock(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_CHIMNEY:
	{
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		int itemId = atoi(tokens[5].c_str());
		int screne_id = atoi(tokens[6].c_str());
		obj = new CChimney(x, y, width, height, itemId,  &objects);
		dynamic_cast<CChimney*>(obj)->SetSceneId(screne_id);
		break;
	}
	case OBJECT_TYPE_SUPER_LEAF: obj = new CSuperLeaf(x, y); break;

	// HUD
	case OBJECT_TYPE_POINT: obj = new CPoint(x, y); break;
	case OBJECT_TYPE_PLAYER_FONT: 
	{
		int scene_id = atoi(tokens[3].c_str());
		obj = new CPlayerFont(x, y);
		choosePlayer = (CPlayerFont*)obj;
		choosePlayer->SetSceneId(scene_id);
		break;
	}
	case OBJECT_TYPE_THIRD_FONT: obj = new CThirdFont(x, y); break;

	// INVISIBLE
	case OBJECT_TYPE_INVISIBLE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());

		if (tokens.size() > 6 && atoi(tokens[6].c_str()) == 1) {
			obj = new CTopPlatform(
				x, y,
				cell_width, cell_height, length
			);
		}
		else {
			obj = new CInvisiblePlatform(
				x, y,
				cell_width, cell_height, length
			);
		}

		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_DEAD_LINE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		obj = new CDeadline(
			x, y,
			cell_width, cell_height, length
		);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_MAP_POINT:
	{
		CMapPoint* point = NULL;
		int id = atoi(tokens[4].c_str());
		int dTop = atoi(tokens[5].c_str());
		int dRight = atoi(tokens[6].c_str());
		int dBottom = atoi(tokens[7].c_str());
		int dLeft = atoi(tokens[8].c_str());
		int scene_id = atoi(tokens[9].c_str());
		point = new CMapPoint(id, dTop, dRight, dBottom, dLeft, scene_id);
		point->SetPosition(x, y);
		mapPoints.push_back(point);
		return;
	}


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}


/*
	Parse a line in section [MAPFILE]
*/
void CPlayScene::_ParseSection_MAPFILE(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	map = new CGameMap();
	map->LoadMap(path.c_str());
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];

	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[SETTINGS]") { section = SCENE_SECTION_SETTINGS; continue; };
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[MAPFILE]") { 
			section = SCENE_SECTION_MAPFILE; continue;
		};
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; };

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAPFILE: _ParseSection_MAPFILE(line); break;
		}
	}

	f.close();

	grid = Grid::GetInstance();

	if (player != NULL || marioWorld != NULL) {
		gameBoard = CBoard::GetInstance();
		gameBoard->SetState(BOARD_STATE_START);
	}

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (marioWorld) {
		for (size_t i = 1; i < objects.size(); i++)
		{
			coObjects.push_back(objects[i]);
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(dt, &coObjects);
		}

		if (gameBoard) {
			gameBoard->Update(dt);
		}
		PurgeDeletedObjects();
	};

	if (player) {
		// Update camera to follow mario
		float cx, cy;
		player->GetPosition(cx, cy);
		
		CMario* mario = (CMario*)player;

		vector<LPGAMEOBJECT> gridObjects;

		grid->GetListObject(gridObjects, objects, cx, cy);

		for (size_t i = 1; i < gridObjects.size(); i++)
		{
			coObjects.push_back(gridObjects[i]);
		}

		if (mario->GetState() != MARIO_STATE_DIE) {
			for (size_t i = 0; i < gridObjects.size(); i++)
			{
				gridObjects[i]->Update(dt, &coObjects);
			}
		} else {
			mario->Update(dt, &coObjects);
		}

		CGame* game = CGame::GetInstance();
		cx -= game->GetBackBufferWidth() / 2;
		cy -= game->GetBackBufferHeight() / 2;

		CGame::GetInstance()->SetCamPos(cx, cy);

		if (gameBoard) {
			gameBoard->Update(dt);
		}
		PurgeDeletedObjects();
	}

}

bool compareRenderOrder(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2)
{
	int renderOrder1, renderOrder2;
	obj1->GetRenderOrder(renderOrder1);
	obj2->GetRenderOrder(renderOrder2);
	return (renderOrder1 < renderOrder2);
};

void CPlayScene::Render()
{
	if (player) {
		float cx, cy;
		player->GetPosition(cx, cy);
		vector<LPGAMEOBJECT> gridObjects;

		grid->GetListObject(gridObjects, objects, cx, cy);

		sort(gridObjects.begin(), gridObjects.end(), compareRenderOrder);

		vector<LPGAMEOBJECT> renderAfterMapObjs;

		for (size_t i = 0; i < gridObjects.size(); i++)
		{
			int renderOrder;
			gridObjects[i]->GetRenderOrder(renderOrder);
			if (renderOrder < 0) {
				gridObjects[i]->Render();
			}
			else {
				renderAfterMapObjs.push_back(gridObjects[i]);
			}
		}

		if (map != NULL) {
			map->Render();
		}

		for (size_t i = 0; i < renderAfterMapObjs.size(); i++)
		{
			renderAfterMapObjs[i]->Render();
		}
	}
	else {
		if (map != NULL) {
			map->Render();
		}

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();
	}

	if (gameBoard) {
		gameBoard->Render();
	}
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	choosePlayer = NULL;
	map = NULL;
	marioWorld = NULL;

	for (UINT i = 0; i < mapPoints.size(); i++)
		delete mapPoints[i];
	mapPoints.clear();

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}