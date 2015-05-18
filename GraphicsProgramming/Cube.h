#pragma once
#include "d3dinclude.h"

#include "Texture.h"

#include "Camera.h"

class Cube
{
public:
	Cube();
	~Cube();


	void Initialize(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float DeltaTime, Camera* p_pCamera);
	void Draw();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11Buffer* m_pMatrixBuffer;

	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11InputLayout* m_pInputLayout;

	Texture* m_pTexture;

	float m_TimePassedSinceGameStart;

};