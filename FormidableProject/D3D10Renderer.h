#pragma once
#include "baserenderer.h"
#include <d3d10.h>
#include <d3dx10.h>
#include <DxErr.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "DXGI.lib")

#include <list>
using namespace std;

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)													\
	{																\
		HRESULT hr = (x);											\
		if(FAILED(hr))												\
		{															\
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);		\
		}															\
	}																
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

class CD3D10Renderer : public CBaseRenderer
{
	HRESULT					hr;
	ID3D10Device*			m_pD3DDevice;
	IDXGIFactory*			m_pFactory;
	IDXGISwapChain*			m_pD3DSwapChain;
	DXGI_SWAP_CHAIN_DESC	m_swapDescription;
	ID3D10RenderTargetView*	m_pD3DRenderTargetView;
	ID3D10Texture2D*		m_pD3DBackbuffer;
	DXGI_SAMPLE_DESC		m_MSAA;
	D3D10_TEXTURE2D_DESC	m_D3DDepthStencilDesc;
	ID3D10Texture2D*		m_pDepthStencilBuffer;
	ID3D10DepthStencilView*	m_pDepthStencilView;
	D3D10_VIEWPORT			m_D3DViewport;

	list<DXGI_SAMPLE_DESC>	m_MSAASupportList;
public:
	CD3D10Renderer(void);
	~CD3D10Renderer(void);
	void Init(HWND hWnd, bool bWindowed, int width, int height);	
	void PreRender(void);	
	void Render(void);		
	void PostRender(void);	
	void Shutdown(void);

	bool CheckMultiSampleSupport(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors and Modifiers
	//////////////////////////////////////////////////////////////////////////
	ID3D10Device* GetDevice() {return m_pD3DDevice;}

};
