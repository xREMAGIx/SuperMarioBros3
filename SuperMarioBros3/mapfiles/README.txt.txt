Format for tiled id txt
numRows numCols numTilesCols cellW cellH spacing
[....id]


Calcutation and draw
Push list tile
for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			inFile >> listTile[i][j];
		}
	}

Get from texture
int index = 0;
for (int i = 0; i < numRows; i++)
{
	for (int j = 0; j < numCols; j++)
	{
		int leftPoint = (index % numTilesCols);
		int topPoint = (index / numTilesCols);
		CSprites::GetInstance()->Add(index++, leftPoint * cellW, topPoint * cellH, (leftPoint + 1) * cellW, (topPoint + 1) * cellH,
			CTextures::GetInstance()->Get(1));
	}
}

Draw map 
for (int i = 0; i < numRows; i++)
{
	for (int j = 0; j < numCols; j++)
	{
		float posX = j * (cellW - 1);
		float posY = i * (cellH - 1);

		if (posX < 0 || posY < 0) continue;

		if (CSprites::GetInstance()->Get(getTitle(i, j)) != NULL)
			CSprites::GetInstance()->Get(getTitle(i, j))->Draw(posX, posY);
	}
}