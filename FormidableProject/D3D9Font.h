#pragma once
#include "BaseFont.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxerr.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif



#include <vector>
using namespace std;


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
	#define HR(x)													\
	{																\
		HRESULT hr = (x);											\
		if(FAILED(hr))												\
		{															\
			DXTrace __FILE__, (DWORD)__LINE__, hr, L#x, true);		\
		}															\
	}																
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

class CD3D9Font : public CBaseFont
{
	IDirect3DDevice9*	m_pD3DDevice;
	vector <ID3DXFont*> m_fontList;
	int					m_fontIndex;
	ID3DXSprite*		m_pSprite;

public:
	CD3D9Font(void);
	~CD3D9Font(void);
	void Init(IDirect3DDevice9* device);
	virtual unsigned int LoadFont(wstring fontName, int fontSize, int fontWidth = 0, bool bold = false, bool italics = false);
	virtual void Shutdown();
	virtual void RenderText(int fontIndex, wstring textOut, int xPos, int yPos);
	virtual void RenderText(int fontIndex, wstring textOut, int xPos, int yPos, int width, int height, unsigned int flags = DT_LEFT | DT_RIGHT, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

};
