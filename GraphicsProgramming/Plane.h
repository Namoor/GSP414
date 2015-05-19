#pragma once

#include "d3dinclude.h"


class Plane
{
public:
	Plane();
	~Plane();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_XSize, int p_YSize);


	void Update(float DeltaTime);
	void Draw();


private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11Buffer* m_pMatrixConstantBuffer;


	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};