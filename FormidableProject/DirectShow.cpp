#include ".\directshow.h"

CDirectShow::CDirectShow(void)
{
}

CDirectShow::~CDirectShow(void)
{
}

bool CDirectShow::Init(HWND hWnd)
{
	// Store the hWnd
	m_hWnd = hWnd;
	m_bVideoPlaying = false;

	CoInitialize(NULL);
	// Initialize Filter Graph
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_graphBuilder);
	if(FAILED(hr))
		return false;
	// Initialize Media Control
	m_graphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_mediaControl);
	// Initialize Media Event
	m_graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_mediaEvent);
	// Initialize Video Window
	m_graphBuilder->QueryInterface(IID_IVideoWindow, (void**)&m_videoWindow);

	// Successful Initialization
	return true;
}

int CDirectShow::LoadFile(char* Filename)
{
	// Convert char* to wide character
	wchar_t formatedFilename[256];
	MultiByteToWideChar(CP_ACP, 0, Filename, -1, formatedFilename, 256);

	// Render the file passed in (this does not play the file)
	m_graphBuilder->RenderFile(formatedFilename, NULL);

	// Give the video Window our Window information
	m_videoWindow->put_Owner((OAHWND)m_hWnd);

	// Set up the video window style
	m_videoWindow->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	// Calculate screen space
	RECT rect;
	GetClientRect(m_hWnd, &rect);

	// Set the view space for our video
	m_videoWindow->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom);

	// Save our initial resolution
	m_screenRect = rect;

	// TODO: Return index of this video
	return 0;
}

void CDirectShow::Play(void)
{
	m_bVideoPlaying = true;
	m_mediaControl->Run();
}

void CDirectShow::Stop(void)
{
	m_bVideoPlaying = false;
	m_mediaControl->Stop();
}

void CDirectShow::Pause(void)
{
	m_mediaControl->Pause();
}
bool CDirectShow::IsFileFinished(void)
{
	long evCode, param1, param2;
	HRESULT hr;

	if(hr = m_mediaEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
	{
		hr = m_mediaEvent->FreeEventParams(evCode, param1, param2);

		if(evCode == EC_COMPLETE)
		{
			m_bVideoPlaying = false;
			return true;
		}
	}

	// Video is not finished
	return false;
}

void CDirectShow::Update(void)
{


	// Readjust the displaying of the video if you window has been changed
	if(m_bVideoPlaying)
	{
		// Calculate screen space
		RECT rect;
		GetClientRect(m_hWnd, &rect);

		if(rect.right != m_screenRect.right || rect.bottom != m_screenRect.bottom)
		{
			// Set the view space for our video
			m_videoWindow->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom);
			m_screenRect = rect;
		}
	}

	// Check and see if the file has finished rendering
	if(IsFileFinished())
	{
		Shutdown();
	}
}

void CDirectShow::Shutdown(void)
{
	// DirectShow component shutdown
	if(m_videoWindow)
	{
		m_videoWindow->Release();
		m_videoWindow = NULL;
	}

	if(m_mediaControl)
	{
		m_mediaControl->Release();
		m_mediaControl = NULL;
	}

	if(m_mediaEvent)
	{
		m_mediaEvent->Release();
		m_mediaEvent = NULL;
	}

	if(m_graphBuilder)
	{
		m_graphBuilder->Release();
		m_graphBuilder = NULL;
	}
}