#include "GameMap.h"

#define MAP_SECTION_UNKNOWN -1
#define MAP_SECTION_READ_DATA 301
#define MAP_SECTION_MAP 302

CGameMap* CGameMap::__instance = NULL;

CGameMap::CGameMap()
{
	listTile[0][0] = 0;
	tilesetId = 0;
	numTilesCols = 0;
	spacing = 0;
	numRows = 0;
	numCols = 0;
	id = 0;
	cellW = 0;
	cellH = 0;
	currentRow = 0;
}


CGameMap::~CGameMap()
{
}

RECT CGameMap::GetSourceRect(int _index)
{
	//DebugOut(L"[INFO] ID: %d \n", _index);
	RECT srect;
	srect.left = (_index % 73) * 16 + 1;
	srect.top = (_index / 73) * 16 + 1;
	srect.right = srect.left + 16;
	srect.bottom = srect.top + 16 + 1;
	return srect;
}

void CGameMap::_ParseSection_READDATA(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 10) return; // skip invalid lines

	LPANIMATION ani = new CAnimation();

	int tile_id = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int r = atoi(tokens[2].c_str());
	int g = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());

	numRows = atoi(tokens[5].c_str());
	numCols = atoi(tokens[6].c_str());
	numTilesCols = atoi(tokens[7].c_str());
	cellW = atoi(tokens[8].c_str());
	cellH = atoi(tokens[9].c_str());
	spacing = atoi(tokens[10].c_str());

	tilesetId = tile_id;

	currentRow = 0;

	DebugOut(L"[INFO] Done loading map tiles %s\n", path.c_str());
}

void CGameMap::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);
	for (UINT i = 0; i < tokens.size(); i++)
	{
		int tileId = atoi(tokens[i].c_str());
		listTile[currentRow][i] = tileId;
	}
	currentRow += 1;
}

void CGameMap::LoadMap(LPCWSTR filepath)
{
	ifstream f;
	f.open(filepath);

	// current resource section flag
	int section = MAP_SECTION_UNKNOWN;

	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[READDATA]") { section = MAP_SECTION_READ_DATA; continue; }
		if (line == "[MAP]") {
			section = MAP_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = MAP_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case MAP_SECTION_READ_DATA: _ParseSection_READDATA(line); break;
		case MAP_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	if (tilesetId != 0) {
		int index = 0;
		LPTEXTURE tex = CTextures::GetInstance()->Get(tilesetId);

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				int leftPoint = (index % numTilesCols);
				int topPoint = (index / numTilesCols);

				CSprites::GetInstance()->Add(index++,
					leftPoint * (cellW + spacing),
					topPoint * (cellH + spacing),
					(leftPoint + 1) * (cellW + spacing) - 1,
					(topPoint + 1) * (cellH + spacing) - 1,
					tex);
			}
		}
	}
}

int CGameMap::getTitle(int x, int y)
{
	return listTile[x][y];
}

void CGameMap::Render()
{

	CGame* game = CGame::GetInstance();
	float camX, camY;
	game->GetCamPos(camX, camY);

	float l = camX - 32,
		t = camY,
		r = camX + game->GetScreenWidth(),
		b = camY + game->GetScreenHeight();

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			float posX = j * cellW * 1.0f;
			float posY = i * cellH * 1.0f;

			if (posX < l || posX > r || posY > b || posY < t) continue;

			if (CSprites::GetInstance()->Get(getTitle(i, j)) != NULL)
				CSprites::GetInstance()->Get(getTitle(i, j))->Draw(posX, posY);
		}
	}
}

CGameMap* CGameMap::GetInstance()
{
	if (__instance == NULL) __instance = new CGameMap();
	return __instance;
}