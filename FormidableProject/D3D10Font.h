#pragma once
#include "basefont.h"
#include <d3d10.h>
#include <d3dx10.h>
#include <DxErr.h>

#ifdef _DEBUG
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dxerr.lib")

#include <vector>
using namespace std;

class CD3D10Font : public CBaseFont
{
public:
	ID3D10Device*			m_pD3DDevice;
	vector <ID3DX10Font*>	m_fontList;
	int						m_fontIndex;
	ID3DX10Sprite*			m_pSprite;


public:
	CD3D10Font(void);
	~CD3D10Font(void);
	void Init(ID3D10Device* device);
	virtual unsigned int LoadFont(wstring fontName, int fontSize, int fontWidth = 0, bool bold = false, bool italics = false);
	virtual void Shutdown();
	virtual void RenderText(int fontIndex, wstring textOut, int xPos, int yPos);
	virtual void RenderText(int fontIndex, wstring textOut, int xPos, int yPos, int width, int height, unsigned int flags = DT_LEFT | DT_RIGHT, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

};
