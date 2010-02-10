#include "D3D9ModelManager.h"

CD3D9ModelManager::CD3D9ModelManager(void)
{
	m_ModelIndex = 0;
}

CD3D9ModelManager::~CD3D9ModelManager(void)
{
}

void CD3D9ModelManager::Init(IDirect3DDevice9* device)
{
	m_pD3DDevice = device;
}

int CD3D9ModelManager::LoadXModel(wstring fileName)
{
	// Create a new model instance
	ModelData* model = new ModelData;

	// Buffers for assisting with the loading of the .x mesh
	ID3DXBuffer* adjacencyBuffer = 0;
	ID3DXBuffer* materialBuffer = 0;
	ID3DXBuffer* effectsBuffer = 0;

	// Load .x mesh
	HRESULT hr = D3DXLoadMeshFromX(fileName.c_str(), 0, m_pD3DDevice, &adjacencyBuffer, 
		&materialBuffer, &effectsBuffer, &model->numMaterials, &model->mesh);

	// If model creation failed, delete the memory we just created, return error code -1
	if(FAILED(hr))
	{
		delete model;
		return -1;
	}
	//ID3DXEffect* effects = (ID3DXEffect*) effectsBuffer->GetBufferPointer();
	
	// Load in materials from material buffer, if there are any materials
	if(materialBuffer && model->numMaterials)
	{
		D3DXMATERIAL* materials = (D3DXMATERIAL*) materialBuffer->GetBufferPointer();

		// Loop through the materials
		for(DWORD i = 0; i < model->numMaterials; ++i)
		{
			D3DMATERIAL9 material;
			material = materials[i].MatD3D;
			//material.Ambient = materials[i].MatD3D.Diffuse;
			material.Diffuse = materials[i].MatD3D.Diffuse;
			material.Specular = materials[i].MatD3D.Specular;
			material.Power = materials[i].MatD3D.Power;

			// Add the material to the list
			model->materialList.push_back(material);

			// If this subset material has an associated texture, load it
			if(materials[i].pTextureFilename)
			{
				// This function sucks, gotta do this bullshit for it to work
				IDirect3DTexture9* texture = 0;
				string texFN = materials[i].pTextureFilename;
				wstring textureFilename(texFN.begin(), texFN.end());
				
				// Load the texture
				D3DXCreateTextureFromFile(m_pD3DDevice, textureFilename.c_str(), &texture);

				// Add the texture to this list
				model->textureList.push_back(texture);
			}
			else
			{
				// No texture for this subsets material
				model->textureList.push_back(0);
			}
		}
	}

	// Add model to model list
	m_ModelList.push_back(model);

	// Done with com object buffers, release them here
	if(adjacencyBuffer)
	{
		adjacencyBuffer->Release();
		adjacencyBuffer = 0;
	}
	if(materialBuffer)
	{
		materialBuffer->Release();
		materialBuffer = 0;
	}
	if(effectsBuffer)
	{
		effectsBuffer->Release();
		effectsBuffer = 0;
	}

	// Return the index to the model data
	return m_ModelIndex++;
}
void CD3D9ModelManager::RenderXModel(int modelIndex)
{
	// Render the .x mesh
	for(DWORD i = 0; i < m_ModelList[modelIndex]->numMaterials; ++i)
	{
		m_pD3DDevice->SetMaterial(&m_ModelList[modelIndex]->materialList[i]);
		if(m_ModelList[modelIndex]->textureList[i])
			m_pD3DDevice->SetTexture(0, m_ModelList[modelIndex]->textureList[i]);
		m_ModelList[modelIndex]->mesh->DrawSubset(i);
	}
}

void CD3D9ModelManager::DestroyModel(int modelIndex)
{
	if(m_ModelList[modelIndex]->mesh)
	{
		m_ModelList[modelIndex]->mesh->Release();
		m_ModelList[modelIndex]->mesh = 0;
		delete m_ModelList[modelIndex]->mesh;
	}
}

void CD3D9ModelManager::ShutDown()
{
	for(int i = 0; i != m_ModelList.size(); ++i)
	{
		if(m_ModelList[i]->mesh)
		{
			m_ModelList[i]->mesh->Release();
			m_ModelList[i]->mesh = 0;
			delete m_ModelList[i]->mesh;
			m_ModelIndex--;
		}
	}
}
