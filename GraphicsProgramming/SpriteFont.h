#pragma once

#include "d3dinclude.h"
#include "Texture.h"
#include "CharDesc.h"


class SpriteFont
{
public:
	SpriteFont(char* p_pFilename, ID3D11Device* p_pGraphicsDevice);
	~SpriteFont();

	Texture* m_pSpriteSheet;
	CharDesc m_Chars[256];
};