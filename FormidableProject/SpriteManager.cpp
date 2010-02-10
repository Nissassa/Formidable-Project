#include "SpriteManager.h"

//	Initialize the static variable member of this singleton class
CSpriteManager*	CSpriteManager::m_pInstance = 0;

CSpriteManager::CSpriteManager()
{
	SpriteListIndex = 0;
}

CSpriteManager::~CSpriteManager(void)
{
	
}

CSpriteManager* CSpriteManager::GetInstance(void)
{
	// Create instance of the class if it has not been created.
	if (!m_pInstance)
		m_pInstance = new CSpriteManager;

	//	Return the address of the instance.
	return m_pInstance;
}

void CSpriteManager::DeleteInstance(void)
{
	// Delete instance of the class if it has been created.
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

void CSpriteManager::Init(IDirect3DDevice9* device)
{	
	m_pD3DDevice = device;
	
	// Create the sprite object
	if(FAILED(D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite)))
	{
		logger->OutputErrorConsole("SpriteManager.cpp - m_pD3DSprite failed to be created.");
	}

	// Set the folder path where sprites are contained in relation to the root folder
	spriteFilePath = L"./Sprites/";
}

void CSpriteManager::Update()
{
	for(int i = 0; i < SpriteListIndex; ++i)
	{
		if(spriteList[i]->doCollision)
		{
			// Update bounding box for each sprite, box based on position with center point offset.
			spriteList[i]->boundingBox.left = LONG(spriteList[i]->position.x - spriteList[i]->center.x);
			spriteList[i]->boundingBox.top = LONG(spriteList[i]->position.y - spriteList[i]->center.y);
			spriteList[i]->boundingBox.right = LONG(spriteList[i]->position.x + spriteList[i]->center.x);
			spriteList[i]->boundingBox.bottom = LONG(spriteList[i]->position.y + spriteList[i]->center.y);
		}
	}

}

void CSpriteManager::Render()
{
	// Called before any sprites can be rendered
	if(SUCCEEDED(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE)))
	{
		//////////////////////////////////////////////////////////////////////////
		// Render all sprites
		//////////////////////////////////////////////////////////////////////////
		D3DXMATRIX worldMat, transMat, scaleMat, rotMat;

		for(int i = 0; i < SpriteListIndex; ++i)
		{
			if(spriteList[i]->doDraw)
			{
				D3DXMatrixIdentity(&worldMat);
				D3DXMatrixIdentity(&transMat);
				D3DXMatrixIdentity(&scaleMat);
				D3DXMatrixIdentity(&rotMat);

				D3DXMatrixTranslation(&transMat, spriteList[i]->position.x, spriteList[i]->position.y, spriteList[i]->position.z);
				D3DXMatrixScaling(&scaleMat, spriteList[i]->scale.x, spriteList[i]->scale.y, 1.0f);
				D3DXMatrixRotationZ(&rotMat, spriteList[i]->rotation);

				D3DXMatrixMultiply(&rotMat, &rotMat, &scaleMat);
				D3DXMatrixMultiply(&worldMat, &rotMat, &transMat);
				m_pD3DSprite->SetTransform(&worldMat);

				m_pD3DSprite->Draw(spriteList[i]->texture, 0, &spriteList[i]->center, 0, spriteList[i]->color);
			}
		}

		m_pD3DSprite->Flush();

		//////////////////////////////////////////////////////////////////////////
		// Called after all sprites have been rendered
		m_pD3DSprite->End();
	}
	else
	{
		logger->OutputErrorConsole("SpriteManager.cpp - m_pD3DSprite->Begin() failed to run.");
	}
	

}

void CSpriteManager::Shutdown()
{
	// Release each sprite texture object and delete the sprite object
	ShutdownSprites();

	// Release the Sprite Object
	if(m_pD3DSprite)
	{
		m_pD3DSprite->Release();
		m_pD3DSprite = NULL;
	}
}

int CSpriteManager::LoadSprite(wstring fileName)
{
	// Create a new sprite object
	Sprite* sprite = new Sprite;
	sprite->fileName = fileName;

	// Check to see if this texture is already loaded
	for(int i = 0; i < SpriteListIndex; ++i)
	{
		if(!fileName.compare(spriteList[i]->fileName))
		{
			// Point to the texture already loaded in memory
			sprite->texture = spriteList[i]->texture;
			sprite->fileName = fileName;
			sprite->imageInfo = spriteList[i]->imageInfo;
			sprite->center = spriteList[i]->center;
		}
	}

	// Append the path to the sprite folder to the filename passed in
	spriteFilePath += fileName;

	// Load the texture into memory
	if(FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, spriteFilePath.c_str(), &sprite->texture)))
	{
		logger->OutputErrorConsole("SpriteManager.cpp - Sprite texture failed to load.");
	}
	// Get Image Info
	D3DXGetImageInfoFromFile(spriteFilePath.c_str(), &sprite->imageInfo);
	// Store the center point of the image
	sprite->center.x = (float)sprite->imageInfo.Width / 2;
	sprite->center.y = (float)sprite->imageInfo.Height / 2;
	sprite->center.z = 0.0f;

	// Calculate bounding box, position based on center point.
	sprite->doDraw = true;
	sprite->doCollision = false;

	sprite->boundingBox.left = LONG(sprite->position.x - sprite->center.x);
	sprite->boundingBox.top = LONG(sprite->position.y - sprite->center.y);
	sprite->boundingBox.right = LONG(sprite->position.x + sprite->center.x);
	sprite->boundingBox.bottom = LONG(sprite->position.y + sprite->center.y);


	// Add the new sprite to the sprite vector
	spriteList.push_back(sprite);
	// Return the index into the vector that the new sprite resides 
	// and then increment for next use
	return SpriteListIndex++;
	
}

void CSpriteManager::ModifySprite(int spriteIndex, bool doDraw, D3DXVECTOR3 pos, D3DXVECTOR3 scale, float rotation, D3DCOLOR color)
{
	spriteList[spriteIndex]->doDraw = doDraw;
	spriteList[spriteIndex]->position = pos;
	spriteList[spriteIndex]->scale = scale;
	spriteList[spriteIndex]->rotation = rotation;
	spriteList[spriteIndex]->color = color;
}

void CSpriteManager::SetSpriteDoDraw(int spriteIndex, bool doDraw)
{
	spriteList[spriteIndex]->doDraw = doDraw;

}

void CSpriteManager::SetSpritePosition(int spriteIndex, D3DXVECTOR3 pos)
{
	spriteList[spriteIndex]->position = pos;
}

void CSpriteManager::SetSpriteScale(int spriteIndex, D3DXVECTOR3 scale)
{
	spriteList[spriteIndex]->scale = scale;
}

void CSpriteManager::SetSpriteRotation(int spriteIndex, float rotation)
{
	spriteList[spriteIndex]->rotation = rotation;
}

void CSpriteManager::SetSpriteColor(int spriteIndex, D3DCOLOR color)
{
	spriteList[spriteIndex]->color = color;
}
bool CSpriteManager::GetSpriteDoDraw(int spriteIndex)
{
	return spriteList[spriteIndex]->doDraw;
}

D3DXVECTOR3 CSpriteManager::GetSpritePosition(int spriteIndex)
{
	return spriteList[spriteIndex]->position;
}

D3DXVECTOR3 CSpriteManager::GetSpriteScale(int spriteIndex)
{
	return spriteList[spriteIndex]->scale;
}

float CSpriteManager::GetSpriteRotation(int spriteIndex)
{
	return spriteList[spriteIndex]->rotation;
}

D3DCOLOR CSpriteManager::GetSpriteColor(int spriteIndex)
{
	return spriteList[spriteIndex]->color;
}


void CSpriteManager::ShutdownSprites()
{
	// Release each sprite texture object
	for(int i = 0; i < SpriteListIndex; ++i)
	{
		if(spriteList[i]->texture)
			spriteList[i]->texture->Release();

		delete spriteList[i];
	}
}

void CSpriteManager::onLostDevice()
{
	m_pD3DSprite->OnLostDevice();
}

void CSpriteManager::onResetDevice()
{
	m_pD3DSprite->OnResetDevice();
}
