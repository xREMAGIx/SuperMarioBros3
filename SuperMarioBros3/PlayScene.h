#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameMap.h"
#include "Block.h"
#include "Mario.h"
#include "Goomba.h"
#include "InvisibleBlock.h"
#include "QuestionBlock.h"
#include "Brick.h"
#include "Coin.h"
#include "Cloud.h"
#include "GreenKoopa.h"
#include "RedKoopa.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"
#include "InvisibleWall.h"
#include "Chimney.h"
#include "EnemyWall.h"
#include "TreeWorld.h"
#include "MarioWorld.h"
#include "MapPoint.h"
#include "ThirdFont.h"
#include "PlayerFont.h"
#include "GameBoard.h"
#include "InvisiblePlatform.h"
#include "MusicBox.h"
#include "VenusFireTrap.h"
#include "BoomerangBrother.h"
#include "Deadline.h"
#include "helpers.h"

class CPlayerPoint
{
public:
	float x;
	float y;

	CPlayerPoint(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

class CPlayScene : public CScene
{
protected:
	CPlayerFont* choosePlayer;
	CMario* player;					// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;
	CMarioWorld* marioWorld;
	vector<CMapPoint*> mapPoints;
	vector<CPlayerPoint*> playerPoints;

	int currentPlayerPoint = 0;
	int currentMapPoint = 0;
	int changeScene = -1;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }
	CMarioWorld* GetMarioWorld() { return marioWorld; }
	CPlayerFont* GetChoosePlayer() { return choosePlayer; }

	vector<LPGAMEOBJECT> GetObjectList() { return objects; }
	void SetObjectList(vector<LPGAMEOBJECT> objects) { objects = this->objects;}

	vector<CMapPoint*> GetMapPoints() { return mapPoints; }
	int GetCurrentMapPoint() { return currentMapPoint; }
	void SetCurrentMapPoint(int index) { this->currentMapPoint = index; }

	int GetChangeScene() { return changeScene; }
	void SetChangeScene(int index) { this->changeScene = index; }

	vector<CPlayerPoint*> GetPlayerPoints() { return playerPoints; }
	int GetCurrentPlayerPoint() { return currentPlayerPoint; }
	void SetCurrentPlayerPoint(int index) { this->currentPlayerPoint = index; }
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode); 
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
