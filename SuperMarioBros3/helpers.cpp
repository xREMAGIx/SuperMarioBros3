#include "helpers.h"
#include "debug.h"

void LoadSprites(CSprites* sprites, const char* fileDir, LPDIRECT3DTEXTURE9 texture) {
	std::fstream file;
	file.open(fileDir, ios::in);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			stringstream ssin(line);
			int id, type, left, top, right, bottom;
			ssin >> id;
			ssin >> type;
			ssin >> left;
			ssin >> top;
			ssin >> right;
			ssin >> bottom;
			sprites->Add(id, left, top, right, bottom, texture);
		}
		file.close();
	}
}

void LoadAnimations(CAnimations* animations, const char* fileDir) {
	std::fstream file;
	LPANIMATION ani;
	file.open(fileDir, ios::in);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			stringstream ssin(line);
			DebugOut(L"[INFO] NEWLINE\n");
			int spriteId;
			int id, time;
			ssin >> id;
			ssin >> time;
			ani = new CAnimation(time);
			while (true)
			{
				ssin >> spriteId;
				if (!ssin)
					break;
				DebugOut(L"[INFO] RUN: %d", spriteId);
				ani->Add(spriteId);
			}
			animations->Add(id, ani);
		}
		file.close();
	}
}