#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <unordered_map>

using namespace std;

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Texture.h"
#include "KeyEventHandler.h"
#include "Scene.h"
#include "debug.h"

#define MAX_FRAME_RATE 100
#define KEYBOARD_BUFFER_SIZE 1024
#define KEYBOARD_STATE_SIZE 256

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)
/*
	Our simple game framework
*/
class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	LPD3DX10SPRITE spriteObject;						// Sprite handling object, BIG MYSTERY: it has to be in this place OR will lead to access violation in D3D11.dll ????

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[KEYBOARD_STATE_SIZE];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	//Camera
	float cam_x = 0.0f;
	float cam_y = 0.0f;

	float min_cam_x = 0.0f;
	float min_cam_y = 0.0f;

	float max_cam_x = 0.0f;
	float max_cam_y = 0.0f;

	//Screen
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	D3DXCOLOR backgroundColor = BACKGROUND_COLOR;

	HINSTANCE hInstance;

	ID3D10SamplerState* pPointSamplerState;

	unordered_map<int, LPSCENE> scenes;
	int previous_scene;
	int current_scene;
	int next_scene = -1;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	// Init DirectX, Sprite Handler
	void Init(HWND hWnd, HINSTANCE hInstance);

	//
	// Draw a portion or ALL the texture at position (x,y) on the screen. (x,y) is at the CENTER of the image
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, LPTEXTURE tex, RECT* rect = NULL, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0);

	void Draw(float x, float y, LPTEXTURE tex, int l, int t, int r, int b, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0)
	{
		RECT rect;
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		this->Draw(x, y, tex, &rect, alpha, sprite_width, sprite_height);
	}

	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	// Keyboard related functions 
	void InitKeyboard();
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }


	ID3D10Device* GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView; }

	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }

	ID3D10BlendState* GetAlphaBlending() { return pBlendStateAlpha; };

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	static CGame* GetInstance();

	void SetPointSamplerState();

	void SetCamPos(float x, float y) {
		cam_x = x * 1.0f;
		cam_y = y * 1.0f;

		if (cam_x < min_cam_x)
			cam_x = min_cam_x;

		if (cam_y > min_cam_y)
			cam_y = min_cam_y;
		
		if (cam_x > max_cam_x - screen_width ) {
			cam_x = max_cam_x - screen_width;
		}

		if (cam_y < max_cam_y - screen_height) {
			cam_y = max_cam_y - screen_height;
		}

		//DebugOut(L"[INFO] cam_x %f\n", cam_x);
		//DebugOut(L"[INFO] cam_y %f\n", cam_y);

	}
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }

	void GetMaxCamScreen(float& x, float& y) { x = max_cam_x; y = max_cam_y; }
	void SetMaxCamScreen(float x, float y) { max_cam_x = x; max_cam_y = y; }

	void GetMinCamScreen(float& x, float& y) { x = min_cam_x; y = min_cam_y; }
	void SetMinCamScreen(float x, float y) { min_cam_x = x; min_cam_y = y; }

	//Screen
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	void SetScreenSize(int screenWidth, int screenHeight) { screen_width = screenWidth; screen_height = screenHeight; }

	D3DXCOLOR GetBackgroundColor() { return backgroundColor; }
	void SetBackgroundColor(D3DXCOLOR color) { backgroundColor = color; }

	LPSCENE GetCurrentScene() { return scenes[current_scene]; }

	int GetCurrentSceneId() { return current_scene; }

	int GetPreviousSceneId() { return previous_scene; }
	void SetPreviousSceneId(int prev) { previous_scene = prev; }

	void Load(LPCWSTR gameFile);
	void SwitchScene();
	void InitiateSwitchScene(int scene_id);

	void _ParseSection_TEXTURES(string line);

	bool isCollision(RECT r1, RECT r2);

	~CGame();
};
typedef CGame* LPGAME;

