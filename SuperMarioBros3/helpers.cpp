#include "helpers.h"
#include "debug.h"
#include "Animations.h"

void LoadSprites(LPCWSTR line, LPDIRECT3DTEXTURE9 texture) {
	CSprites* sprites = CSprites::GetInstance();

	ifstream f;
	f.open(line);
	char str[4096];

	while (f.getline(str, 4096))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		vector<string> tokens = split(line);
		if (tokens.size() < 6) return; // skip invalid lines

		int id = atoi(tokens[0].c_str());
		int type = atoi(tokens[1].c_str());
		int left = atoi(tokens[2].c_str());
		int top = atoi(tokens[3].c_str());
		int right = atoi(tokens[4].c_str());
		int bottom = atoi(tokens[5].c_str());

		sprites->Add(id, left, top, right, bottom, texture);
	}

	f.close();
	/*
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
	*/

}

void LoadAnimations(LPCWSTR line) {
	CAnimations* animations = CAnimations::GetInstance();

	ifstream f;
	f.open(line);
	char str[2048];

	while (f.getline(str, 2048))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		vector<string> tokens = split(line);

		if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

		int id = atoi(tokens[0].c_str());
		DebugOut(L"[INFO] Load animation id %d\n", id);

		int time = atoi(tokens[1].c_str());
		LPANIMATION ani = new CAnimation();
		DebugOut(L"[INFO] Load token size %d\n", tokens.size());

		for (int i = 2; i < tokens.size(); i++ )
		{
			int sprite_id = atoi(tokens[i].c_str());

			ani->Add(sprite_id, time);
		}
		animations->Add(id, ani);

	}

	f.close();
	/*
	DebugOut(L"[INFO] FILEDIR: %d", line);
	std::fstream file;
	LPANIMATION ani;
	file.open(fileDir, ios::in);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			stringstream ssin(line);
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
				ani->Add(spriteId);
			}
			animations->Add(id, ani);
		}
		file.close();
	}
	*/
}

void DebugOut(wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

int GetDistanceX(RECT r1, RECT r2)
{
	int x1 = (r1.left + r1.right) / 2,
		x2 = (r2.left + r2.right) / 2;

	return abs(x2 - x1);
}

int GetDistanceY(RECT r1, RECT r2)
{
	int y1 = (r1.top + r1.bottom) / 2,
		y2 = (r2.top + r2.bottom) / 2;

	return abs(y2 - y1);
}

int GetRectW(RECT r)
{
	return r.right - r.left;
}

int GetRectH(RECT r)
{
	return r.bottom - r.top;
}
