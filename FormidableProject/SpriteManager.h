//////////////////////////////////////////////////////////////////////////
// File Name:	SpriteManager.h
// Author:		Kyle Lauing
// Date:		10/27/2008
// Purpose:		This is to manage the rendering of 2D sprites for Menus
//				and HUD objects.  
//////////////////////////////////////////////////////////////////////////
#pragma once

//////////////////////////////////////////////////////////////////////////
// Headers and Libraries
//////////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "Logger.h"

class CSpriteManager
{
	// Singleton Instance
	static CSpriteManager*		m_pInstance;
	// Logger instance
	CLogger*					logger;
	// D3D Device
	IDirect3DDevice9*			m_pD3DDevice;

	// Sprite Object
	ID3DXSprite*				m_pD3DSprite;

	// Sprite Structure
	struct Sprite
	{
		// Sprite texture
		IDirect3DTexture9*	texture;
		D3DXIMAGE_INFO		imageInfo;
		wstring				fileName;

		// Sprite Object properties
		D3DXVECTOR3			position;
		D3DXVECTOR3			center;
		D3DXVECTOR3			scale;
		float				rotation;
		D3DCOLOR			color;
		bool				doDraw;
		bool				doCollision;
		RECT				boundingBox;
	};

	wstring spriteFilePath;

	vector <Sprite*> spriteList;
	int SpriteListIndex;

public:
	CSpriteManager();
	~CSpriteManager(void);

	// Singleton functions
	static CSpriteManager* GetInstance(void);
	static void DeleteInstance(void);

	void Init(IDirect3DDevice9* device);
	void Update();
	void Render();
	void Shutdown();

	int LoadSprite(wstring fileName);
	
	// Modify all sprite object parameters on a single sprite
	void ModifySprite(int spriteIndex, bool doDraw, D3DXVECTOR3 pos, D3DXVECTOR3 scale, float rotation, D3DCOLOR color);
	
	// Modify one specific sprite object parameter on a single sprite
	void SetSpriteDoDraw(int spriteIndex, bool doDraw);
	void SetSpritePosition(int spriteIndex, D3DXVECTOR3 pos);
	void SetSpriteScale(int spriteIndex, D3DXVECTOR3 scale);
	void SetSpriteRotation(int spriteIndex, float rotation);
	void SetSpriteColor(int spriteIndex, D3DCOLOR color);
	
	// Access one specific sprite object parameter on a single sprite
	bool GetSpriteDoDraw(int spriteIndex);
	D3DXVECTOR3 GetSpritePosition(int spriteIndex);
	D3DXVECTOR3 GetSpriteScale(int spriteIndex);
	float GetSpriteRotation(int spriteIndex);
	D3DCOLOR GetSpriteColor(int spriteIndex);


	void ShutdownSprites();

	void onLostDevice();
	void onResetDevice();



};
