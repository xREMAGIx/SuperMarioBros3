#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Grid.h"

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

GameMap* map;
Grid* grid;
CBoard* gameBoard;

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	DebugOut(L"[INFO] [RUN SETTINGS]\n");

	CGame* game = CGame::GetInstance();

	vector<string> tokens = split(line);

	if (tokens.size() < 9) return; // skip invalid lines

	int scene_width = atoi(tokens[0].c_str());
	int scene_height = atoi(tokens[1].c_str());
	int cam_x = atoi(tokens[2].c_str());
	int cam_y = atoi(tokens[3].c_str());
	int maxCamX = atoi(tokens[4].c_str());
	int maxCamY = atoi(tokens[5].c_str());
	int r = atoi(tokens[6].c_str());
	int g = atoi(tokens[7].c_str());
	int b = atoi(tokens[8].c_str());

	game->SetCamPos(cam_x, cam_y);
	game->SetMaxCamPos(maxCamX, maxCamY);
	game->SetBackgroundColor(r, g, b);
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
	case OBJECT_TYPE_QUESTION_BLOCK: { 
		int itemId = atof(tokens[4].c_str());
		int aniSetId = atof(tokens[5].c_str());
		obj = new CQuestionBlock(itemId, aniSetId);
		break; 
	}
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
	case OBJECT_TYPE_INVISIBLE_PLATFORM:
	{
		int width = atof(tokens[4].c_str());
		obj = new CInvisiblePlatform(width);
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
		int itemId = atof(tokens[6].c_str());
		int aniSetId = atof(tokens[7].c_str());
		obj = new CChimney(width, height, itemId, aniSetId, x, y, &objects);
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
		int scene_id = atof(tokens[9].c_str());
		point = new CMapPoint(id, dTop, dRight, dBottom, dLeft, scene_id);
		point->SetPosition(x, y);
		mapPoints.push_back(point);
		return;
	}
	case OBJECT_TYPE_INTRO_3_FONT: {
		obj = new CThirdFont();
		break;
	}
	case OBJECT_TYPE_MUSIC_BOX: {
		obj = new CMusicBox();
		int id = atof(tokens[4].c_str());
		if (id == 1) {
			obj->SetState(MUSIC_BOX_STATE_HIDDEN);
		}
		break;
	}
	case OBJECT_TYPE_INTRO_PLAYER_FONT: {
		int scene_id = atof(tokens[4].c_str());
		obj = new CPlayerFont();
		choosePlayer = (CPlayerFont*)obj;
		choosePlayer->SetSceneId(scene_id);
		break;
	}
	case OBJECT_TYPE_VENUS_FIRE_TRAP: {
		obj = new CVenusFireTrap();
		break;
	}
	case OBJECT_TYPE_BOOMERANG_BROTHER: {
		obj = new CBoomerangBrother();
		break;
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetId(object_type);
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
	grid = Grid::GetInstance();
	gameBoard = CBoard::GetInstance();
	if (player != NULL) {
		gameBoard->SetState(BOARD_STATE_START);
	}
	
	if (marioWorld != NULL) {
		vector<CMapPoint*> map_points = ((CPlayScene*)this)->GetMapPoints();
		marioWorld->SetMovingPoint(map_points.at(currentMapPoint)->x, map_points.at(currentMapPoint)->y);

		//Set current point
		marioWorld->SetPosition(map_points.at(currentMapPoint)->x, map_points.at(currentMapPoint)->y);
	}
	
}

void CPlayScene::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();

	if (player != NULL) {
		float cx, cy;
		player->GetPosition(cx, cy);

		CGame* game = CGame::GetInstance();
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;

		CGame::GetInstance()->SetCamPos(cx, cy);

		vector<LPGAMEOBJECT> coObjects;

		grid->GetListObject(coObjects, objects, cx, cy);

		coObjects[0]->Update(dt, &coObjects);

		for (size_t i = 1; i < coObjects.size(); i++)
		{
			coObjects[i]->Update(dt, &coObjects);
		}

		gameBoard->Update(dt);
	}

	else if (marioWorld != NULL) {
		marioWorld->Update(dt);
		float cx, cy;
		marioWorld->GetPosition(cx, cy);

		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;

		CGame::GetInstance()->SetCamPos(cx, cy);
		gameBoard->Update(dt);
		
	}
	else {
		if (choosePlayer == NULL) return;

		choosePlayer->Update(dt);

		float cx, cy;
		choosePlayer->GetPosition(cx, cy);

		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;

		CGame::GetInstance()->SetCamPos(cx, cy);
	}
}

void CPlayScene::Render()
{
	//map->Render();

	CGame* game = CGame::GetInstance();
	D3DXVECTOR2 cam = game->GetCamPos();

	CSprites* sprites = CSprites::GetInstance();

	float l = cam.x,
		t = cam.y,
		r = cam.x + game->GetScreenWidth(),
		b = cam.y + game->GetScreenHeight();

	vector<CChimneyLayout*> listChimneyMap;

	int init_row = (l / map->cellW);
	int end_row = (r / map->cellW);
	int init_col = (t / map->cellH);
	int end_col = (b / map->cellH);

	
	for (int i = init_col; i < end_col; i++)
	{
		for (int j = init_row; j < end_row; j++)
		{
			int posX = j * map->cellW;
			int posY = i * map->cellH;

			if (sprites->Get(map->getTitle(i, j)) != NULL) {
				switch (map->getTitle(i, j))
				{
				case 150:
				case 151:
				case 223:
				case 224: {
					CChimneyLayout* chimney_block = new CChimneyLayout(map->getTitle(i, j), posX, posY);
					listChimneyMap.push_back(chimney_block);
					break;
				}
				default:
					sprites->Get(map->getTitle(i, j))->Draw(posX, posY);
					break;
				}
				
			}
		}
	}
	

	for (int i = 0; i < objects.size(); i++) {
		float posX = objects[i]->x;
		float posY = objects[i]->y;

		if (posX < l - 32 || posX > r || posY > b || posY < t) continue;
		objects[i]->Render();
	}

	if (listChimneyMap.size() > 0) {
		for (int i = 0; i < listChimneyMap.size(); i++) {
			listChimneyMap[i]->Render();
		}
	}

	if (choosePlayer != NULL) {
		choosePlayer->Render();
	}
	else {
		gameBoard->Render();
	}
	
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
	choosePlayer = NULL;

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
	CPlayerFont* choosePlayer = ((CPlayScene*)scence)->GetChoosePlayer();

	int current_point = ((CPlayScene*)scence)->GetCurrentMapPoint();

	if (mario != NULL) {
		// disable control key when Mario die 
		if (mario->GetState() == MARIO_STATE_DIE) return;
		switch (KeyCode)
		{
			case DIK_X:
				if (mario->GetState() != MARIO_STATE_JUMP && mario->GetState() != MARIO_STATE_JUMP_RIGHT && mario->GetState() != MARIO_STATE_JUMP_LEFT) {
					mario->SetState(MARIO_STATE_JUMP);
				}
				break;
			case DIK_1:
				mario->SetLevel(MARIO_LEVEL_SMALL);
				break;
			case DIK_2:
				mario->SetLevel(MARIO_LEVEL_BIG);
				break;
			case DIK_3:
				mario->SetLevel(MARIO_LEVEL_TAIL);
				break;
			case DIK_4:
				mario->SetLevel(MARIO_LEVEL_FIRE);
				break;
			case DIK_Z:
				if (mario->GetLevel() == MARIO_LEVEL_TAIL || mario->GetLevel() == MARIO_LEVEL_FIRE) {
					mario->SetState(MARIO_STATE_ATTACK);
					break;
				}
		}
	}
	else if (mario_world != NULL) {
		if (mario_world->GetState() == MARIO_WORLD_STATE_MOVING) return;
		switch (KeyCode)
		{
			case DIK_RIGHT:
			{
				CMapPoint* point = map_points.at(current_point);
				
				if (point->GetDRight() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDRight());


					mario_world->MovingPosition(nextPoint->x, nextPoint->y);

					//mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDRight());
				}
				break;
			}
			case DIK_LEFT:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetDLeft() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDLeft());

					mario_world->MovingPosition(nextPoint->x, nextPoint->y);

					//mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDLeft());
				}
				break;
			}
			case DIK_DOWN:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetDBottom() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDBottom());

					mario_world->MovingPosition(nextPoint->x, nextPoint->y);

					//mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDBottom());
				}
				break;
			}
			case DIK_UP:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetDTop() != -1) {
					CMapPoint* nextPoint = map_points.at(point->GetDTop());

					mario_world->MovingPosition(nextPoint->x, nextPoint->y);

					//mario_world->SetPosition(nextPoint->x, nextPoint->y);
					((CPlayScene*)scence)->SetCurrentMapPoint(point->GetDTop());
				}
				break;
			}
			case DIK_X:
			{
				CMapPoint* point = map_points.at(current_point);

				if (point->GetSceneId() != -1) {
					CGame::GetInstance()->SwitchScene(point->GetSceneId());
				}
				break;
			}
		}
	}
	else {
		switch (KeyCode)
		{
			case DIK_DOWN:
				choosePlayer->SetState(PLAYER_FONT_STATE_TWO);
				break;
			case DIK_UP:
				choosePlayer->SetState(PLAYER_FONT_STATE_ONE);
				break;
			case DIK_X:
				CGame::GetInstance()->SwitchScene(choosePlayer->GetSceneId());
				break;
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMarioWorld* mario_world = ((CPlayScene*)scence)->GetMarioWorld();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if(mario != NULL) {
	// disable control key when Mario die 
		if (mario->GetState() == MARIO_STATE_DIE || mario->GetState() == MARIO_STATE_ATTACK) return;

		if (mario->GetState() != MARIO_STATE_JUMP && mario->GetState() != MARIO_STATE_JUMP_RIGHT && mario->GetState() != MARIO_STATE_JUMP_LEFT) {
		
			if ((game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT)) && game->IsKeyDown(DIK_Z)) {
				if (mario->GetState() != MARIO_STATE_HOLDING) {
					mario->SetState(MARIO_STATE_RUNNING);
				}
			}
			else {
				if (game->IsKeyDown(DIK_RIGHT)) {
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
				}
				else if (game->IsKeyDown(DIK_LEFT)) {
					mario->SetState(MARIO_STATE_WALKING_LEFT);
				}
				else if (game->IsKeyDown(DIK_DOWN)) {
					mario->SetState(MARIO_STATE_DOWN);
				}
				else {
					mario->SetState(MARIO_STATE_IDLE);
				}
			
			}
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
