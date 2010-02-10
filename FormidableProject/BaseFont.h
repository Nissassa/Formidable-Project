#pragma once
#include <d3dx9.h>
#include <d3dx10.h>
#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#endif

#include <string>
using namespace std;

class CBaseFont
{
public:
	//CBaseFont(void);
	//virtual ~CBaseFont(void) {}
	//virtual void Init();
	virtual unsigned int LoadFont(wstring fontName, int fontSize, int fontWidth = 0, bool bold = false, bool italics = false) = 0;
	virtual void Shutdown() = 0;
	virtual void RenderText(int fontIndex, wstring textOut, int xPos, int yPos) = 0;
	virtual void RenderText(int fontIndex, wstring textOut, int xPos, int yPos, int width, int height, unsigned int flags = DT_LEFT | DT_RIGHT, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) = 0;

};
