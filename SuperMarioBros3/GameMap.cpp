#include "GameMap.h"


#define MAP_SECTION_UNKNOWN -1
#define MAP_SECTION_READ_DATA 1
#define MAP_SECTION_MAP 2

GameMap* GameMap::__instance = NULL;

GameMap::GameMap()
{
}


GameMap::~GameMap()
{
}

RECT GameMap::GetSourceRect(int _index)
{
	//DebugOut(L"[INFO] ID: %d \n", _index);
	RECT srect;
	srect.left = (_index % 73) * 16 + 1;
	srect.top = (_index / 73) * 16 + 1;
	srect.right = srect.left + 16; 
	srect.bottom = srect.top + 16 + 1;
	return srect;
}

void GameMap::_ParseSection_READDATA(string line)
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

	CTextures* textures = CTextures::GetInstance();
	textures->Add(tile_id, path.c_str(), D3DCOLOR_XRGB(r, g, b));
	currentRow = 0;

	DebugOut(L"[INFO] Done loading map tiles %s\n", path.c_str());
}

void GameMap::_ParseSection_MAP(string line)
{
	DebugOut(L"[INFO] [RUN MAP ID ROW] %d\n", currentRow);
	vector<string> tokens = split(line);
	int index;
	for (int i = 0; i < tokens.size(); i++)
	{
		int tileId = atoi(tokens[i].c_str());
		listTile[currentRow][i] = tileId;
	}
	currentRow += 1;
}

void GameMap::LoadMap(LPCWSTR filepath)
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

	int index = 0;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			int leftPoint = (index % numTilesCols);
			int topPoint = (index / numTilesCols);

			CSprites::GetInstance()->Add(index++,
				leftPoint * (cellW + spacing),
				topPoint * (cellH + spacing),
				(leftPoint + 1) * (cellW + spacing),
				(topPoint + 1) * (cellH + spacing),
				CTextures::GetInstance()->Get(tilesetId));
		}
	}
}
  
int GameMap::getTitle(int x, int y)
{
	return listTile[x][y];
}

void GameMap::Render()
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			float posX = j * cellW;
			float posY = i * cellH;

			if (posX < 0 || posY < 0) continue;

			if (CSprites::GetInstance()->Get(getTitle(i, j)) != NULL)
				CSprites::GetInstance()->Get(getTitle(i, j))->Draw(posX, posY);
		}
	}
}

GameMap* GameMap::GetInstance()
{
	if (__instance == NULL) __instance = new GameMap();
	return __instance;
}
