#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

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

#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_TREE_WORLD 100
#define OBJECT_TYPE_MARIO_WORLD 101
#define OBJECT_TYPE_MAP_POINT 102


#define MAX_SCENE_LINE 1024

GameMap* map;
Quadtree* quadtree;

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	DebugOut(L"[INFO] [RUN SETTINGS]\n");

	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int scene_width = atoi(tokens[0].c_str());
	int scene_height = atoi(tokens[1].c_str());
	int cam_x = atoi(tokens[2].c_str());
	int cam_y = atoi(tokens[3].c_str());
	int maxCamX = atoi(tokens[4].c_str());
	int maxCamY = atoi(tokens[5].c_str());

	max_cam_x = maxCamX;
	max_cam_y = maxCamY;

	CGame::GetInstance()->ReInit(scene_width, scene_height);
	CGame::GetInstance()->SetCamPos(cam_x, cam_y);
}

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	DebugOut(L"[INFO] [RUN SPRITES]\n");

	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int texID = atoi(tokens[2].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}
	LoadSprites(path.c_str(), tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	DebugOut(L"[INFO] [RUN ANIMATIONS]\n");

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);

	LoadAnimations(path.c_str());

	DebugOut(L"[INFO] Load file animation resources %s\n", path.c_str());
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario();
		obj->SetPosition(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
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
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BLOCK: obj = new CBlock(); break;
	case OBJECT_TYPE_QUESTION_BLOCK: obj = new CQuestionBlock(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(); break;
	case OBJECT_TYPE_CLOUD: obj = new CCloud(); break;
	case OBJECT_TYPE_GREEN_KOOPA: obj = new CGreenKoopa(); break;
	case OBJECT_TYPE_RED_KOOPA: obj = new CRedKoopa(); break;
	case OBJECT_TYPE_PARA_GOOMBA: {
		obj = new CParaGoomba(x, y);
		break;
	} 
	case OBJECT_TYPE_PARA_KOOPA: obj = new CParaKoopa(); break;

	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_INVISIBLE_BLOCK: 
	{
		int width = atof(tokens[4].c_str());
		obj = new CInvisibleBlock(width);
		break;
	}
	case OBJECT_TYPE_INVISIBLE_WALL:
	{
		int height = atof(tokens[4].c_str());
		obj = new CInvisibleWall(height);
		break;
	}
	case OBJECT_TYPE_CHIMNEY:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new CChimney(width, height);
		break;
	}
	case OBJECT_TYPE_ENEMY_WALL:
	{
		int height = atof(tokens[4].c_str());
		obj = new CEnemyWall(height);
		break;
	}
	case OBJECT_TYPE_TREE_WORLD: obj = new CTreeWorld(); break;
	case OBJECT_TYPE_MAP_POINT:
	{
		CMapPoint* point = NULL;
		int id = atof(tokens[4].c_str());
		int dTop = atof(tokens[5].c_str());
		int dRight = atof(tokens[6].c_str());
		int dBottom = atof(tokens[7].c_str());
		int dLeft = atof(tokens[8].c_str());

		point = new CMapPoint(id, dTop, dRight, dBottom, dLeft);
		point->SetPosition(x, y);
		mapPoints.push_back(point);

		return;
	}

	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	if (ani_set_id != 0)
	{
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
	}	
	objects.push_back(obj);
}


void CPlayScene::_ParseSection_MAP(string line)
{
	DebugOut(L"[INFO] [RUN MAP]\n");

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);

	map = new GameMap();
	map->LoadMap(path.c_str());

	DebugOut(L"[INFO] Load file map resources %s\n", path.c_str());
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = SCENE_SECTION_SETTINGS; continue; }
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	//quadtree->CreateQuadTree();
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	
	/*
	vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();

		for (int i = 0; i < coObjects.size(); i++)
		{
			//Get all objects that can collide with current entity
			quadtree->Retrieve(return_objects_list, coObjects[i]);

			for (auto x = return_objects_list->begin(); x != return_objects_list->end(); x++)
			{
				coObjects[i]->Update(dt, return_objects_list);
			}

			return_objects_list->clear();
		}

	quadtree->Clear();

	delete return_objects_list;
	delete quadtree;
	*/
	
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	//place update position camera at final 
	if (cx > max_cam_x - game->GetScreenWidth()) {
		cx = max_cam_x - game->GetScreenWidth();
	}


	if (cy > max_cam_y) {
		cy = max_cam_y;
	}

	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::Render()
{
	map->Render();

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);

	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	map = NULL;
	marioWorld = NULL;

	for (int i = 0; i < mapPoints.size(); i++)
		delete mapPoints[i];

	mapPoints.clear();
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	CMarioWorld* mario_world = ((CPlayScene*)scence)->GetMarioWorld();
	vector<CMapPoint*> map_points = ((CPlayScene*)scence)->GetMapPoints();

	int current_point = ((CPlayScene*)scence)->GetCurrentMapPoint();


	if (mario_world != NULL) {
		switch (KeyCode)
		{
			case DIK_RIGHT:
			{
				CMapPoint* point = map_points.at(current_point);
				
				if (point->GetDRight() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDRight());

					mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDRight());
				}
				break;
			}
			case DIK_LEFT:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetDLeft() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDLeft());

					mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDLeft());
				}
				break;
			}
			case DIK_DOWN:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetDBottom() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDBottom());

					mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDBottom());
				}
				break;
			}
			case DIK_UP:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetDTop() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDTop());

					mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDTop());
				}
				break;
			}
		}
	}
	else {
		switch (KeyCode)
		{
		case DIK_SPACE:
			if (mario->GetState() != MARIO_STATE_JUMP && mario->GetState() != MARIO_STATE_JUMP_RIGHT && mario->GetState() != MARIO_STATE_JUMP_LEFT) {
				mario->SetState(MARIO_STATE_JUMP);
			}
			break;
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMarioWorld* mario_world = ((CPlayScene*)scence)->GetMarioWorld();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (mario_world != NULL) {
		mario_world->SetState(MARIO_WORLD_STATE_SMALL);
	}
	else {

	

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (mario->GetState() != MARIO_STATE_JUMP && mario->GetState() != MARIO_STATE_JUMP_RIGHT && mario->GetState() != MARIO_STATE_JUMP_LEFT) {
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		else
			mario->SetState(MARIO_STATE_IDLE);
	}
	else {
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->SetState(MARIO_STATE_JUMP_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->SetState(MARIO_STATE_JUMP_LEFT);
		}
	}

	}
}
