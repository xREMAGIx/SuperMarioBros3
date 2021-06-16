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
