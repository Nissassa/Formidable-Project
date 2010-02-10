#include "D3D10ModelManager.h"

CD3D10ModelManager::CD3D10ModelManager(void)
{
}

CD3D10ModelManager::~CD3D10ModelManager(void)
{
}

void CD3D10ModelManager::Init(ID3D10Device* device)
{
	m_pD3DDevice = device;
}

int CD3D10ModelManager::LoadXModel(wstring fileName)
{
	return 0;
}
void CD3D10ModelManager::RenderXModel(int modelIndex)
{

}
void CD3D10ModelManager::DestroyModel(int modelIndex)
{
}

void CD3D10ModelManager::ShutDown()
{
}
