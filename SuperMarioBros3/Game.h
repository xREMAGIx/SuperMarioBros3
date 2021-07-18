#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include "KeyEventHandler.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Scene.h"
#include "PlayScene.h"
#include "helpers.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object
	D3DPRESENT_PARAMETERS d3dpp;

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	//Input
	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;


	//Camera
	float cam_x = 0.0f;
	float cam_y = 0.0f;

	float max_cam_x = 0.0f;
	float max_cam_y = 0.0f;

	//Screen
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	D3DCOLOR background_color = BACKGROUND_COLOR;

	//Scene
	unordered_map<int, LPSCENE> scenes;
	int current_scene;
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);


public:
	//Sprite
	void Init(HWND hWnd);
	void ReInit(int scene_width, int scene_height);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);
	void DrawFlipX(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);
	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor);
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	D3DPRESENT_PARAMETERS GetParameters() { return d3dpp; }
	HWND GetHWND() { return hWnd; }
	LPDIRECT3D9 GetDirectHandle() { return d3d; }

	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	//Input
	void InitKeyboard();
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }

	//Collision
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	//Camera
	void SetCamPos(float x, float y)
	{
		cam_x = x;
		cam_y = y;
		if (cam_x < 0)
			cam_x = 0;
		if (cam_y < 0)
			cam_y = 0;

		if (cam_x > max_cam_x - screen_width) {
			cam_x = max_cam_x - screen_width;
		}

		if (cam_y > max_cam_y) {
			cam_y = max_cam_y;
		}

	}
	void SetMaxCamPos(float x, float y) {
		max_cam_x = x;
		max_cam_y = y;
	}

	D3DXVECTOR2 GetCamPos()
	{
		return D3DXVECTOR2(cam_x, cam_y);
	}
	static CGame* GetInstance();

	bool isCollision(RECT r1, RECT r2);

	//Load Data
	void Load(LPCWSTR gameFile);

	//Scene
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	//Screen
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	D3DCOLOR GetBackgroundColor() { return background_color; }
	void SetBackgroundColor(int r, int g, int b) { background_color = D3DCOLOR_XRGB(r, g, b); }

	~CGame();
};