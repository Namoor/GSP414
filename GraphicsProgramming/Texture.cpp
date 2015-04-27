#include "Texture.h"



Texture::Texture(char* p_pFilename, ID3D11Device* p_pDevice)
{
	D3DX11CreateShaderResourceViewFromFile(p_pDevice, p_pFilename, nullptr, nullptr, &m_pSRV, nullptr);
}