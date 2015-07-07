#pragma once

#include "d3dinclude.h"
#include "Camera.h"
#include "Texture.h"


class ShadingDemo
{
public:
	ShadingDemo();
	~ShadingDemo();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);


	void Update(float DeltaTime, Camera* p_pCamera);
	void Draw();


	float GetHeightAt(float x, float y);

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11Buffer* m_pMatrixConstantBuffer;

	ID3D11Buffer* m_pPixelConstantBuffer;

	float m_OffsetFactor;

	ID3D11SamplerState* m_pTextureSampler;

	Texture* m_pTexture;

	int IndexCount;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};