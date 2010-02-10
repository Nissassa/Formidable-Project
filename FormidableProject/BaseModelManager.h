#pragma once
#include <d3dx9.h>
#include <d3dx10.h>

#ifdef _DEBUG
	#pragma comment(lib, "d3dx9d.lib")
	#pragma comment(lib, "d3dx10d.lib")
#else
	#pragma comment(lib, "d3dx9.lib")
	#pragma comment(lib, "d3dx10.lib")
#endif

#include <string>
#include <vector>
using namespace std;

class CBaseModelManager
{
public:
	//CBaseModelManager(void);
	//virtual ~CBaseModelManager(void);
	//virtual void Init(/*D3DDevice Type*/);
	virtual int LoadXModel(wstring fileName) = 0;
	virtual void RenderXModel(int modelIndex) = 0;
	virtual void DestroyModel(int modelIndex) = 0;
	virtual void ShutDown() = 0;
};
