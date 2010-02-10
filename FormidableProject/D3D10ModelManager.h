#pragma once
#include "basemodelmanager.h"

class CD3D10ModelManager : public CBaseModelManager
{
	ID3D10Device*			m_pD3DDevice;

public:
	CD3D10ModelManager(void);
	~CD3D10ModelManager(void);
	void Init(ID3D10Device* device);
	int LoadXModel(wstring fileName);
	void RenderXModel(int modelIndex);
	void DestroyModel(int modelIndex);
	void ShutDown();

};
