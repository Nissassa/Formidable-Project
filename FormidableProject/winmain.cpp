//////////////////////////////////////////////////////////////////////////
// File Name:	Winmain.cpp
// Author:		Kyle Lauing
// Date:		11/28/2009
// Purpose:		Contains the WinMain() entry point for our program.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#define VC_EXTRALEAN
#include "GameManager.h"
#include "Logger.h"

//////////////////////////////////////////////////////////////////////////
// Defines and Globals - Quickly change the default window parameters
//////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH 1280						// Window Resolution Width
#define SCREEN_HEIGHT 768						// Window Resolution Height
#define WINDOW_TITLE L"The Formidable Project"	// Window Title/Class String
#define WINDOWED_MODE true						// Windowed Mode or Full screen?

HINSTANCE		g_hAppInstance;					// Handle to our windows instance
HWND			g_hWindow;						// Handle to our window
CLogger*		m_Logger;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// Handle Windows Messages
	switch(message)
	{
	//case (WM_SIZE):
	//	{
	//		if(wparam == SIZE_MINIMIZED)
	//		{
	//			bPaused = true;
	//		}
	//		else
	//		{
	//			bPaused = false;
	//		}
	//		break;
	//	}
	//case (WM_ENTERSIZEMOVE):
	//	{
	//		bPaused = true;
	//		break;
	//	}
	//case (WM_ENTERMENULOOP):
	//	{
	//		bPaused = true;
	//		break;
	//	}
	//case (WM_ACTIVATEAPP):
	//	{
	//		if(wparam == TRUE)
	//		{
	//			bPaused = false;
	//		}
	//		break;
	//	}
	//case (WM_EXITSIZEMOVE):
	//	{
	//		bPaused = true;
	//		break;
	//	}
	//case (WM_EXITMENULOOP):
	//	{
	//		bPaused = true;
	//		break;
	//	}
	case (WM_DESTROY):
		{
			PostQuitMessage(0);
			break;
		}
	case (WM_PAINT):
		{
			InvalidateRect(hWnd,NULL,TRUE);
		}
	case (WM_KEYDOWN):
		{
			switch(wparam)
			{
			case VK_ESCAPE:
				{
					PostQuitMessage(0);
					break;
				}
			}

		}
	}
	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}

bool InitWindow(void)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	// Set up the Window
	wndClass.cbSize			= sizeof(WNDCLASSEX);			// Size of window structure
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;				// Message callback
	wndClass.lpszClassName	= WINDOW_TITLE;					// Class name
	wndClass.hInstance		= g_hAppInstance;				// Handle to the application
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);	// Default cursor
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);	// Background brush

	// register a new type of window
	RegisterClassEx(&wndClass);

	g_hWindow = CreateWindow(
		WINDOW_TITLE,											// Class name
		WINDOW_TITLE,											// Window title
		WINDOWED_MODE ? WS_OVERLAPPEDWINDOW | WS_VISIBLE:(WS_POPUP | WS_VISIBLE),// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,							// X and Y coordinates
		SCREEN_WIDTH, SCREEN_HEIGHT,							// Width and Height of window
		NULL, NULL,												// Parent window and menu
		g_hAppInstance,											// Handle to application
		NULL);

	// Display the window
	ShowWindow(g_hWindow, SW_SHOW);
	UpdateWindow(g_hWindow);

	// Return success
	return true;

}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{ 
	//////////////////////////////////////////////////////////////////////////
	// Initialize debugging support systems
	//////////////////////////////////////////////////////////////////////////
	m_Logger = CLogger::GetInstance();
	m_Logger->Init();
	m_Logger->OutputErrorConsole("Debugging Logger Initialized.");
	
	// Store application handle
	g_hAppInstance = hInstance;

	// Init the window
	if( !InitWindow() )
		return 0;

	// Game Manager Initialization
	GameManager	gameManager;	// Instance of Game Manager
	gameManager.startUp(g_hWindow, g_hAppInstance);

	// Use this to catch windows messages
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	// Main Windows/Game loop
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Game Manager run
		gameManager.run();

	}

	// Shutdown Game Manager
	gameManager.shutDown();

	// Unregister window
	UnregisterClass(WINDOW_TITLE, g_hAppInstance);

	// Return successful
	return 0;

}