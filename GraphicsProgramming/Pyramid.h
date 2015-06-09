#pragma once

#include "Camera.h"
#include "Texture.h"
#include "d3dinclude.h"

class Pyramid
{
public:
	Pyramid();
	~Pyramid();

public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext);

	void Update(float p_DeltaTime, Camera* p_pCamera);
	void Draw();


private:

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Buffer* m_pConstantBuffer;

	ID3D11InputLayout* m_pInputLayout;

	Texture* m_pTexture;
};