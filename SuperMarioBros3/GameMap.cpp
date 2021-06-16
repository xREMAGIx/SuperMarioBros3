#include "GameMap.h"


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

void GameMap::LoadMap(const char* filepath)
{
	ifstream inFile;
	inFile.open(filepath);

	inFile >> numRows;
	inFile >> numCols;
	inFile >> numTilesCols;
	inFile >> cellW;
	inFile >> cellH;
	inFile >> spacing;

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			inFile >> listTile[i][j];
		}
	}
	/*
	std::ifstream  file;
	file.open(filepath, ios::in);
	if (file.is_open()) {
		for (int i = 0; i < 155; i++)
		{
			for (int j = 0; j < 27; j++) 
			{
				int id;
				file >> id;
				listTile[i][j] = id;
				DebugOut(L"[INFO] LOADMAP: %d", id);
			}
		}

		file.close();
	}
	*/

	CTextures* textures = CTextures::GetInstance();
	textures->Add(1, L"mapfiles\\stagetiles.png", D3DCOLOR_XRGB(224, 163, 216));
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
				CTextures::GetInstance()->Get(1));
		}
	}
}
  
int GameMap::getTitle(int x, int y)
{
	return listTile[x][y];
}

void GameMap::DrawMap()
{
	/*
	int rowCam = CGame::GetInstance()->GetCamPos().y / 16;
	int colCam = CGame::GetInstance()->GetCamPos().x / 16;

	int x = (int)CGame::GetInstance()->GetCamPos().x % 16;
	int y = (int)CGame::GetInstance()->GetCamPos().y % 16;

	for (int i = 0; i < SCREEN_HEIGHT / 16; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH / 16 + 1; j++)
		{
			//DebugOut(L"[INFO] RECT: %d %d\n", i + rowCam, j + colCam);
			RECT rect = GetSourceRect(listTile[i + rowCam][j + colCam]);
			//DebugOut(L"[INFO] DRAWPLACE: %d %d\n", j * 16 - x, i * 16 - y + 32);
			CGame::GetInstance()->Draw(j * 16 - x, i * 16 - y, CTextures::GetInstance()->Get(ID_TEX_MAP1), rect.left, rect.top, rect.right, rect.bottom);
			//CGame::GetInstance()->Draw(j * 16 - x, i * 16 - y, CTextures::GetInstance()->Get(ID_TEX_MAP1), 16, 16, 32, 32);
		}
	}
	*/

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

