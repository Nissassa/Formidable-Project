#include "GameManager.h"

GameManager::GameManager(void)
{
	// Do nothing
	m_nWidth = 1280;
	m_nHeight = 768;
	m_bWindowed = true;
}

GameManager::~GameManager(void)
{
	// Do nothing
}

void GameManager::startUp(HWND hwnd, HINSTANCE hInst)
{
	// Store some windows variables within this class
	m_hWnd = hwnd;
	m_hInstance = hInst;

	////////////////////////////////////////////////////////////////////////////
	//// Initialize debugging support systems
	////////////////////////////////////////////////////////////////////////////
	m_Logger = CLogger::GetInstance();
	//m_Logger->Init();
	m_Logger->OutputErrorConsole("Game Manager Initialized!");
	// Start Timer class
	m_Timer.Start();
	//////////////////////////////////////////////////////////////////////////
	// Initialize rendering subsystems
	// TODO: Figure out an elegant way to switch between different supported
	// major systems on a particular platform, i.e. by scanning hardware	
	//////////////////////////////////////////////////////////////////////////

	// Specify Direct3D Implementation (D3D9, D3D10, D3D11)
	m_RenderSelection = D3D9;

	m_CameraManager = new CCameraManager;
	m_CameraManager->SetProjectionMatrix(80.0f, (float)m_nWidth/(float)m_nHeight, 1.0f, 500.0f);
	m_CameraManager->CreateCamera(D3DXVECTOR3(0.0f, 2.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//	m_CameraManager->CreateCamera(D3DXVECTOR3(10.0f, 2.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//	m_CameraManager->CreateCamera(D3DXVECTOR3(-10.0f, 2.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	// For specific Direct3D Implementations
	switch(m_RenderSelection)
	{
	case D3D9:
		{
			m_Renderer = new CD3D9Renderer;
			m_FontManager = new CD3D9Font;
			m_ModelManager = new CD3D9ModelManager;

			CD3D9Renderer* m_Renderer9 = dynamic_cast<CD3D9Renderer*>(m_Renderer);
			m_Renderer9->Init(m_hWnd, m_bWindowed, m_nWidth, m_nHeight);

			m_FontManager9 = dynamic_cast<CD3D9Font*>(m_FontManager);
			m_FontManager9->Init(m_Renderer9->GetDevice());

			m_ModelManager9 = dynamic_cast<CD3D9ModelManager*>(m_ModelManager);
			m_ModelManager9->Init(m_Renderer9->GetDevice());


			//////////////////////////////////////////////////////////////////////////
			// Temporary Camera and Light Fixed Function Pipeline Calls
			//////////////////////////////////////////////////////////////////////////
			// This will be normally passed to shader functions with each object's world matrix
			m_Renderer9->GetDevice()->SetTransform(D3DTS_VIEW, &m_CameraManager->GetViewMat());
			m_Renderer9->GetDevice()->SetTransform(D3DTS_PROJECTION, &m_CameraManager->GetProjMat());

			// This light wont exist when the software shader lights are functional
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(light));
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Range = 1000.0f;
			light.Direction = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
			light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_Renderer9->GetDevice()->SetLight(0, &light);
			m_Renderer9->GetDevice()->LightEnable(0, true);
			//////////////////////////////////////////////////////////////////////////
			break;
		}
	case D3D10:
		{
			m_Renderer = new CD3D10Renderer;
			m_FontManager = new CD3D10Font;
			m_ModelManager = new CD3D10ModelManager;

			CD3D10Renderer* m_Renderer10 = dynamic_cast<CD3D10Renderer*>(m_Renderer);
			m_Renderer10->Init(m_hWnd, m_bWindowed, m_nWidth, m_nHeight);

			m_FontManager10 = dynamic_cast<CD3D10Font*>(m_FontManager);
			m_FontManager10->Init(m_Renderer10->GetDevice());

			m_ModelManager10 = dynamic_cast<CD3D10ModelManager*>(m_ModelManager);
			m_ModelManager10->Init(m_Renderer10->GetDevice());
			break;
		}
	}
	

	// Load fonts
	m_FontManager->LoadFont(L"Arial", 20, 0, true);
	m_FontManager->LoadFont(L"Verdana", 20, 0, true);
	m_FontManager->LoadFont(L"Times New Roman", 20, 0, true);

	m_ModelManager->LoadXModel(L"Heli.x");


}

void GameManager::run(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Debugging and Performance Info
	//////////////////////////////////////////////////////////////////////////
	// Update timer, get elapsed time since last frame as well as FPS
	float FrameTime = 0;
	int FPS = 0;
	m_Timer.Update();
	FPS = (int)m_Timer.GetFPS();
	FrameTime = m_Timer.GetElapsedTime() * 1000.0f;

	// Check m_Renderer's type using typeid
	string classIDTemp = typeid(*m_Renderer).name();
	wstring className(classIDTemp.begin() + 5, classIDTemp.end());

	// swprintf_s to format the string to display frame time in ms, and FPS
	wchar_t buffer[128];
	swprintf_s(buffer, 128, L"Frame Time: %f ms\nFPS: %d", FrameTime, FPS);
	wstring output = buffer;
	//////////////////////////////////////////////////////////////////////////

	m_Renderer->PreRender();
	m_Renderer->Render();

	//////////////////////////////////////////////////////////////////////////
	// Rendering and Debugging Test
	//////////////////////////////////////////////////////////////////////////
	m_ModelManager->RenderXModel(0);
	m_FontManager->RenderText(0, output, 0, 0, m_nWidth, m_nHeight, DT_LEFT | DT_TOP, D3DXCOLOR(.8f, .8f, .8f, 1.0f));
	m_FontManager->RenderText(1, output, 0, 100, m_nWidth, m_nHeight, DT_LEFT | DT_TOP, D3DXCOLOR(.8f, .8f, .8f, 1.0f));
	m_FontManager->RenderText(2, output, 0, 200, m_nWidth, m_nHeight, DT_LEFT | DT_TOP, D3DXCOLOR(.8f, .8f, .8f, 1.0f));
	m_FontManager->RenderText(0, className, 0, 0, m_nWidth, m_nHeight, DT_RIGHT | DT_TOP, D3DXCOLOR(1.0f, .3f, .5f, 1.0f));
	//////////////////////////////////////////////////////////////////////////

	m_Renderer->PostRender();
}

void GameManager::shutDown(void)
{
	// Shut down in reverse order as startUp()
	m_ModelManager->ShutDown();
	m_FontManager->Shutdown();
	m_Renderer->Shutdown();

	m_Logger->Shutdown();
	m_Logger->DeleteInstance();

}