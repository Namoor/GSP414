#pragma once

#include "d3dinclude.h"
#include "Camera.h"
#include "Texture.h"
#include "TilingTerrain_Layer.h"

#include "TilingTerrain_Data.h"


class TilingTerrain
{
public:
	TilingTerrain();
	~TilingTerrain();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_XSize, int p_YSize, TilingTerrain_Data* p_pData);


	void Update(float DeltaTime, Camera* p_pCamera);
	void Draw();

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11Buffer* m_pMatrixConstantBuffer;

	ID3D11Buffer* m_pPixelConstantBuffer;
	ID3D11SamplerState* m_pTextureSampler;

	TilingTerrain_Layer* m_pGrass;
	TilingTerrain_Layer* m_pSnow;
	TilingTerrain_Layer* m_pStone;
	TilingTerrain_Layer* m_pCity;


	Texture* m_pGrassTexture;

	int IndexCount;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};