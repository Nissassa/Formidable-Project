#pragma once
#include "baserenderer.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <list>
using namespace std;

#include "Logger.h"

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

class CD3D9Renderer : public CBaseRenderer
{
	//////////////////////////////////////////////////////////////////////////
	// D3D Variables
	//////////////////////////////////////////////////////////////////////////
	HRESULT					hr;
	IDirect3D9*				m_pD3DObject;
	IDirect3DDevice9*		m_pD3DDevice;
	D3DPRESENT_PARAMETERS	m_presentParams;
	D3DCAPS9				m_D3DCaps;
	D3DMULTISAMPLE_TYPE		m_multiSampleType;
	DWORD					m_multiSampleQuality;

	struct MSAA
	{
		D3DMULTISAMPLE_TYPE		type;
		DWORD					quality;
	};
	list<MSAA>				m_MSAASupportList;
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	CLogger*				m_Logger;
	list<D3DDISPLAYMODE>	m_displayModeList;
	HWND					m_hWnd;
	int						m_nWidth, m_nHeight;
	bool					m_bWindowed;
	bool					m_bVsync;
	int						m_nRefreshRate;



public:
	CD3D9Renderer(void);
	~CD3D9Renderer(void);
	 void Init(HWND hWnd, bool bWindowed, int width, int height);	
	 void PreRender(void);	
	 void Render(void);		
	 void PostRender(void);	
	 void Shutdown(void);		

	bool CheckMultiSampleSupport(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors and Modifiers
	//////////////////////////////////////////////////////////////////////////
	IDirect3DDevice9* GetDevice() {return m_pD3DDevice;}
};