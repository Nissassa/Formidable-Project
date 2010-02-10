#pragma once
#include "Logger.h"
#include "D3D9Renderer.h"
#include "D3D9Font.h"
#include "D3D9ModelManager.h"

#include "D3D10Renderer.h"
#include "D3D10Font.h"
#include "D3D10ModelManager.h"

#include "CameraManager.h"

#include "Timer.h"
#include <wchar.h>

class GameManager
{
	//////////////////////////////////////////////////////////////////////////
	// Debugging information
	//////////////////////////////////////////////////////////////////////////
	CLogger*		m_Logger;
	CTimer			m_Timer;
	
	
	//////////////////////////////////////////////////////////////////////////
	// Game Settings and support variables (usually from WinMain)
	//////////////////////////////////////////////////////////////////////////
	bool			m_bWindowed;			// Windowed or Full-Screen
	int				m_nWidth, m_nHeight;	// Window resolution

	HWND			m_hWnd;					// Handle to the window
	HINSTANCE		m_hInstance;			// Handle to application instance

	//////////////////////////////////////////////////////////////////////////
	// Rendering system (TEST)
	//////////////////////////////////////////////////////////////////////////
	enum RENDERTYPE {D3D9, D3D10, D3D11};
	UINT				m_RenderSelection;
	CBaseRenderer*		m_Renderer;		// Implementation TBD by this class, platform detection
	CBaseFont*			m_FontManager;	// Font class
	CD3D9Font*			m_FontManager9;
	CD3D10Font*			m_FontManager10;
	CBaseModelManager*	m_ModelManager;
	CD3D9ModelManager*	m_ModelManager9;
	CD3D10ModelManager*	m_ModelManager10;

	CCameraManager*		m_CameraManager;






public:
	void startUp(HWND hwnd, HINSTANCE hInst);
	void run(void);
	void shutDown(void);

	GameManager(void);
	~GameManager(void);
};
