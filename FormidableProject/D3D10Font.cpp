#include "D3D10Font.h"

CD3D10Font::CD3D10Font(void)
{
}

CD3D10Font::~CD3D10Font(void)
{
}
void CD3D10Font::Init(ID3D10Device* device)
{
	// Store D3D Device pointer
	m_pD3DDevice = device;

	// Create sprite object
	D3DX10CreateSprite(m_pD3DDevice, 128, &m_pSprite);
}

unsigned int CD3D10Font::LoadFont(wstring fontName, int fontSize, int fontWidth, bool bold, bool italics)
{
	ID3DX10Font* font;
	if(bold)
		D3DX10CreateFont(m_pD3DDevice, fontSize, fontWidth, FW_BOLD, 0, italics, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, (LPCWSTR)fontName.c_str(), &font);
	else
		D3DX10CreateFont(m_pD3DDevice, fontSize, fontWidth, 0, 0, italics, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, (LPCWSTR)fontName.c_str(), &font);

	// Add this font to the list of fonts, return index into font list and increment it.
	m_fontList.push_back(font);

	return m_fontIndex++;
}

void CD3D10Font::Shutdown()
{
	for(int i = 0; i != m_fontList.size(); ++i)
	{
		if(m_fontList[i])
		{
			m_fontList[i]->Release();
			m_fontList[i] = 0;
			delete m_fontList[i];
		}

	}
	if(m_pSprite)
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}
}

void CD3D10Font::RenderText(int fontIndex, wstring textOut, int xPos, int yPos)
{
	RECT textRect;
	textRect.left = xPos;
	textRect.top = yPos;
	textRect.right = 100;
	textRect.bottom = 100;
	
	m_pSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	m_fontList[fontIndex]->DrawText(m_pSprite, textOut.c_str(), -1, &textRect, DT_NOCLIP | DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSprite->End();
}

void CD3D10Font::RenderText(int fontIndex, wstring textOut, int xPos, int yPos, int width, int height, unsigned int flags, D3DXCOLOR color)
{
	RECT textRect;
	textRect.left = xPos;
	textRect.top = yPos;
	textRect.right = width;
	textRect.bottom = height;

	m_pSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	m_fontList[fontIndex]->DrawText(m_pSprite, textOut.c_str(), -1, &textRect, flags, color);
	m_pSprite->End();
}