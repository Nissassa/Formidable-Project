#include "D3D9Renderer.h"

CD3D9Renderer::CD3D9Renderer(void)
{
	// Default initial values
	m_pD3DObject			= NULL;
	m_pD3DDevice			= NULL;
	m_multiSampleType		= D3DMULTISAMPLE_NONE;
	m_multiSampleQuality	= 0;
	m_bVsync				= false;
	m_nRefreshRate			= 59;
	m_Logger				= CLogger::GetInstance();
}

CD3D9Renderer::~CD3D9Renderer(void)
{
}

void CD3D9Renderer::Init(HWND hWnd, bool bWindowed, int width, int height)
{
	// Store these variables for D3D
	m_hWnd = hWnd;
	m_bWindowed = bWindowed;
	m_nWidth = width;
	m_nHeight = height;

	//////////////////////////////////////////////////////////////////////////
	// Initialize D3D
	//////////////////////////////////////////////////////////////////////////
	// Set up D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find supported refresh rate/resolutions/buffers of monitor and device
	for(unsigned int i = 0; i < m_pD3DObject->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8); ++i)
	{
		D3DDISPLAYMODE displayMode;

		// Loop through the number of adapters, and call EnumAdapterModes()
		// Add local displayMode to displayModeList
		m_pD3DObject->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &displayMode);
		m_displayModeList.push_back(displayMode);
	}

	// Print all display adapter modes to logger window
	#ifdef _DEBUG
	for(list<D3DDISPLAYMODE>::iterator i = m_displayModeList.begin(); i != m_displayModeList.end(); ++i)
	{
		char buffer[256];
		sprintf_s(buffer, 256, "Width: %u, Height: %u, Refresh: %u, Format: %u", (*i).Width, (*i).Height, (*i).RefreshRate, (*i).Format);
		m_Logger->OutputErrorConsole(buffer);
	}
	#endif // _DEBUG

	// Find supported multi-sampling info (Full-screen anti aliasing)
	if(!CheckMultiSampleSupport())
	{
		// Multi-sampling not supported.
		m_multiSampleQuality = 0;
		m_multiSampleType = D3DMULTISAMPLE_NONE;
	}

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);
	
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// Hardware vertex processing?
	else
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; // If not, use software

	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	// If hardware vertex processing is on, check pure device

	// Set up D3D Device presentation parameters
	ZeroMemory(&m_presentParams, sizeof(m_presentParams));
	m_presentParams.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth and buffer, 24 bit depth, 8 bit stencil
	m_presentParams.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	m_presentParams.BackBufferFormat			= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	m_presentParams.BackBufferHeight			= height;									// Make sure resolution is supported, use adapter modes
	m_presentParams.BackBufferWidth				= width;									// (Same as above)
	m_presentParams.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	m_presentParams.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	m_presentParams.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes
	m_presentParams.hDeviceWindow				= hWnd;										// Handle to the application window
	m_presentParams.MultiSampleQuality			= m_multiSampleQuality;						// Set to highest multi-sample quality
	m_presentParams.MultiSampleType				= m_multiSampleType;						// Set to highest multi-sample type
	m_presentParams.PresentationInterval		= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	m_presentParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	m_presentParams.Windowed					= bWindowed;								// Windowed or Full-screen
	
	// Create D3D Device
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,						// Primary display adapter
		D3DDEVTYPE_HAL,							// Hardware rasterization
		hWnd,									// Focus Window
		deviceBehaviorFlags,					// Device Flags
		&m_presentParams,						// Device Presentation Parameters
		&m_pD3DDevice);							// Returned Device Object

	// Turn on the Z-Buffer (Depth Buffer)
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

void CD3D9Renderer::PreRender(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Clear the back buffer
	//////////////////////////////////////////////////////////////////////////
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 100, 200), 1.0f, 0);

	m_pD3DDevice->BeginScene();
}
	
void CD3D9Renderer::Render(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Render 3D Objects
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Render 2D Objects
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Render Text
	//////////////////////////////////////////////////////////////////////////
}

void CD3D9Renderer::PostRender(void)
{
	m_pD3DDevice->EndScene();
	//////////////////////////////////////////////////////////////////////////
	// Present the back buffer
	//////////////////////////////////////////////////////////////////////////
	m_pD3DDevice->Present(0,0,0,0);
}

void CD3D9Renderer::Shutdown(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Macro to keep code clean, release com objects and set them to NULL
	//////////////////////////////////////////////////////////////////////////
	#define RELEASE(x) if(x){x->Release(); x = NULL;}

	// Release in reverse order as init()

	RELEASE(m_pD3DDevice)
	RELEASE(m_pD3DObject)

}

bool CD3D9Renderer::CheckMultiSampleSupport(void)
{
	for(int i = 0; i <= 16; ++i)
	{
		MSAA msaaTest;
		// Manually check all of these multi sample types
		switch(i)
		{
		case 0:
			msaaTest.type = D3DMULTISAMPLE_NONE;
			break;
		case 1:
			msaaTest.type = D3DMULTISAMPLE_NONMASKABLE;
			break;
		case 2:
			msaaTest.type = D3DMULTISAMPLE_2_SAMPLES;
			break;
		case 3:
			msaaTest.type = D3DMULTISAMPLE_3_SAMPLES;
			break;
		case 4:
			msaaTest.type = D3DMULTISAMPLE_4_SAMPLES;
			break;
		case 5:
			msaaTest.type = D3DMULTISAMPLE_5_SAMPLES;
			break;
		case 6:
			msaaTest.type = D3DMULTISAMPLE_6_SAMPLES;
			break;
		case 7:
			msaaTest.type = D3DMULTISAMPLE_7_SAMPLES;
			break;
		case 8:
			msaaTest.type = D3DMULTISAMPLE_8_SAMPLES;
			break;
		case 9:
			msaaTest.type = D3DMULTISAMPLE_9_SAMPLES;
			break;
		case 10:
			msaaTest.type = D3DMULTISAMPLE_10_SAMPLES;
			break;
		case 11:
			msaaTest.type = D3DMULTISAMPLE_11_SAMPLES;
			break;
		case 12:
			msaaTest.type = D3DMULTISAMPLE_12_SAMPLES;
			break;
		case 13:
			msaaTest.type = D3DMULTISAMPLE_13_SAMPLES;
			break;
		case 14:
			msaaTest.type = D3DMULTISAMPLE_14_SAMPLES;
			break;
		case 15:
			msaaTest.type = D3DMULTISAMPLE_15_SAMPLES;
			break;
		case 16:
			msaaTest.type = D3DMULTISAMPLE_16_SAMPLES;
			break;
		}

		// Can back-buffer support MSAA?
		hr = m_pD3DObject->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,		// Default display adapter
			D3DDEVTYPE_HAL,			// Hardware (HAL) device type
			D3DFMT_X8R8G8B8,		// Surface format
			m_bWindowed,			// Windowed Mode bool
			msaaTest.type,			// Multi-sample type
			&msaaTest.quality);		// Multi-sample quality
		// If failed, MSAA not supported
		if(FAILED(hr))
			continue;

 		// Can the depth-buffer support MSAA?
		hr = m_pD3DObject->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,		// Default display adapter
			D3DDEVTYPE_HAL,			// Hardware (HAL) device type
			D3DFMT_D24S8,			// Surface format
			m_bWindowed,			// Windowed Mode bool
			msaaTest.type,			// Multi-sample type
			&msaaTest.quality);		// Multi-sample quality
		// If failed, MSAA not supported
		if(FAILED(hr))
			continue;

		// Add to list of supported MSAA formats
		msaaTest.quality -= 1;
		m_MSAASupportList.push_back(msaaTest);
	}


	// Multi-sampling supported, highest level quality stored in variables
	if(!m_MSAASupportList.empty())
	{
		m_multiSampleQuality = m_MSAASupportList.back().quality;
		m_multiSampleType = m_MSAASupportList.back().type;
		return true;
	}
	else
		return false;
}
