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
#include "QuadTree.h"
#include "InvisibleWall.h"
#include "helpers.h"


class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

public:
	vector<LPGAMEOBJECT> objects;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }
	vector<LPGAMEOBJECT> GetObjectList() { return this->objects; }

};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode); 
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

