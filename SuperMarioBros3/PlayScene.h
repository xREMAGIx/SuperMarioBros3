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
#include "Cloud.h"
#include "Block.h"
#include "Chimney.h"
#include "SuperLeaf.h"

#include "Goomba.h"
#include "RedGoomba.h"
#include "Koopa.h"
#include "RedKoopa.h"

#include "GameBoard.h"
#include "GameMap.h"
#include "Portal.h"
#include "Coin.h"
#include "Mushroom.h"
#include "GreenMushroom.h"
#include "QuestionBlock.h"
#include "Point.h"
#include "PlayerFont.h"
#include "ThirdFont.h"

#include "MapPoint.h"
#include "InvisiblePlatform.h"
#include "TopPlatform.h"
#include "Deadline.h"

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

	float portalNextPosX = -1;
	float portalNextPosY = -1;
	int portalNextDirection = -1;

	int currentMapPoint = 0;
	int currentMarioLevel = MARIO_LEVEL_SMALL;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPFILE(string line);

	void LoadAssets(LPCWSTR assetFile);

public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadPrevData(
		float portalNextPosX,
		float portalNextPosY,
		int portalNextDirection,
		int currentMarioLevel
	);
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

	void GetPortalNextPos(float& x, float& y, int& direction) {
		x = portalNextPosX; 
		y = portalNextPosY; 
		direction = portalNextDirection;
	}
	void SetPortalNextPos(float x, float y, int direction) { 
		this->portalNextPosX = x;
		this->portalNextPosY = y;
		this->portalNextDirection = direction;
	}

	void GetCurrentMarioLevel(int& level) { level = currentMarioLevel; }
	void SetCurrentMarioLevel(int level) {
		this->currentMarioLevel = level;
	}

	vector<LPGAMEOBJECT>& GetGameObjects() {	
		return this->objects;
	}

	void Clear();
	void PurgeDeletedObjects();
	void ResetPrevData();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

