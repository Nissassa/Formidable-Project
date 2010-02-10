#pragma once
#include "basemodelmanager.h"
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

#include <string>
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

class CD3D9ModelManager : public CBaseModelManager
{
	IDirect3DDevice9*	m_pD3DDevice;
	struct ModelData
	{
		ID3DXMesh*					mesh;
		DWORD						numMaterials;
		int							modelID;
		vector<D3DMATERIAL9>		materialList;
		vector<IDirect3DTexture9*>	textureList;
	};

	vector<ModelData*>	m_ModelList;
	UINT				m_ModelIndex;

public:
	CD3D9ModelManager(void);
	~CD3D9ModelManager(void);
	void Init(IDirect3DDevice9* device);
	int LoadXModel(wstring fileName);
	void RenderXModel(int modelIndex);
	void DestroyModel(int modelIndex);
	void ShutDown();
};
