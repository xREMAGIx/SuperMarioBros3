#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Grid.h"
#include "Brick.h"

#include "Mario.h"
#include "MarioWorld.h"

#include "TreeWorld.h"

#include "Goomba.h"
#include "RedGoomba.h"
#include "Koopa.h"
#include "RedKoopa.h"

#include "PlayerFont.h"
#include "GameBoard.h"
#include "GameMap.h"

#include "MapPoint.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;			
	LPGAMEMAP map;
	CBoard* gameBoard;
	LPPLAYERFONT choosePlayer;
	CMarioWorld* marioWorld;
	vector<CMapPoint*> mapPoints;
	vector<LPGAMEOBJECT> objects;
	Grid* grid;

	int currentMapPoint = 0;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPFILE(string line);

	void LoadAssets(LPCWSTR assetFile);

public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	CBoard* GetGameBoard() { return gameBoard; }
	LPPLAYERFONT GetChoosePlayer() { return choosePlayer; }
	CMarioWorld* GetMarioWorld() { return marioWorld; }

	vector<CMapPoint*> GetMapPoints() { return mapPoints; }
	int GetCurrentMapPoint() { return currentMapPoint; }
	void SetCurrentMapPoint(int index) { this->currentMapPoint = index; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

