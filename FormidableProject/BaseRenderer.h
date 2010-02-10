#pragma once
#include <windows.h>
#define VC_EXTRALEAN

//////////////////////////////////////////////////////////////////////////
// Abstract Base Class for our basic rendering interfaces
//////////////////////////////////////////////////////////////////////////
class CBaseRenderer
{

public:
//	CBaseRenderer(void);
//	~CBaseRenderer(void);

	virtual void Init(HWND hWnd, bool bWindowed, int width, int height)	= 0;
	virtual void PreRender(void)										= 0;
	virtual void Render(void)											= 0;
	virtual void PostRender(void)										= 0;
	virtual void Shutdown(void)											= 0;
};

