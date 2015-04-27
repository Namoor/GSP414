#pragma once

#include "d3dinclude.h"


class Texture
{
public:
	Texture(char* p_pFilename, ID3D11Device* p_pDevice);

public:
	ID3D11ShaderResourceView* m_pSRV;
};