#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma once
#include <sstream>
#include <fstream>
#include <string>
#include "debug.h"
#include "helpers.h"
#include "Game.h"
#include "Textures.h"
#include "Mario.h"
#include "Goomba.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Super Mario Bros 3"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_FRAME_RATE 60

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_HUD 30
#define ID_TEX_INTRO 40

CMario* mario;
CGoomba* goomba;
CGameObject* test;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_START_VX 0.001f

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, L"textures\\mario-luigi.png", D3DCOLOR_XRGB(68, 145, 190));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(68, 145, 190));
	textures->Add(ID_TEX_MISC, L"textures\\npc-misc.png", D3DCOLOR_XRGB(166, 185, 255));
	textures->Add(ID_TEX_INTRO, L"textures\\intro.png", D3DCOLOR_XRGB(255, 255, 255));

	CSprites* sprites = CSprites::GetInstance();

	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);
	LPDIRECT3DTEXTURE9 texEnemies = textures->Get(ID_TEX_ENEMY);
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	LPDIRECT3DTEXTURE9 texIntro = textures->Get(ID_TEX_INTRO);

	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION ani;

	LoadSprites(sprites, "data/character-sprite.txt", texMario);
	LoadSprites(sprites, "data/enemies-sprite.txt", texEnemies);
	LoadSprites(sprites, "data/misc-sprite.txt", texMisc);
	LoadSprites(sprites, "data/intro-sprite.txt", texIntro);

	LoadAnimations(animations, "data/enemies-animation.txt");


	ani = new CAnimation(1000);
	ani->Add(12001);
	ani->Add(12002);
	ani->Add(12003);
	ani->Add(12004);
	//ani->Add(20003);
	animations->Add(503, ani);
	ani = new CAnimation(1000);
	ani->Add(10002);
	ani->Add(10001);
	//ani->Add(20003);
	animations->Add(504, ani);

	ani = new CAnimation(1000);
	ani->Add(20001);
	ani->Add(20002);
	//ani->Add(20003);
	animations->Add(505, ani);

	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX);
	goomba = new CGoomba(MARIO_START_X, MARIO_START_Y);
	test = new CMario(0, 0, 0);
}
 
/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	mario->Update(dt);
	goomba->Update(dt);
}

/*
	Render a frame
*/
void Render()
{
	CGame* game = CGame::GetInstance();
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		mario->Render();
		goomba->Render();

		//
		// TEST SPRITE DRAW
		//

		/*
		CTextures *textures = CTextures::GetInstance();

		D3DXVECTOR3 p(20, 20, 0);
		RECT r;
		r.left = 274;
		r.top = 234;
		r.right = 292;
		r.bottom = 264;
		spriteHandler->Draw(textures->Get(ID_TEX_MARIO), &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
		*/

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();
	Run();

	return 0;
}