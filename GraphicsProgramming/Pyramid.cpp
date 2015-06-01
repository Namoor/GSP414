#include "Pyramid.h"
#include "Pyramid_Structs.h"
#include "Macros.h"

Pyramid::Pyramid()
{
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;

	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;

	m_pInputLayout = nullptr;

	m_pConstantBuffer = nullptr;
}

Pyramid::~Pyramid()
{
	DX_RELEASE(m_pVertexBuffer);
	DX_RELEASE(m_pIndexBuffer);
	DX_RELEASE(m_pVertexShader);
	DX_RELEASE(m_pPixelShader);
	DX_RELEASE(m_pInputLayout);
	DX_RELEASE(m_pConstantBuffer);
}

void Pyramid::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;


	// --------------------------------------------------------- VertexBuffer ----------------------------------------------------


	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBDesc.ByteWidth = sizeof(Pyramid_Vertex) * 5; // Größe von einem Element * Anzahl Elemente
	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	Pyramid_Vertex _Vertices[5];

	D3DXVECTOR3 Color = D3DXVECTOR3(1, 1, 1);

	// Boden
	_Vertices[0].Position = D3DXVECTOR3(-0.5f, 0, -0.5f);
	_Vertices[0].Color = Color;
	_Vertices[0].Normal = D3DXVECTOR3(0, -1, 0);
	_Vertices[0].UV = D3DXVECTOR2(0, 0);

	_Vertices[1].Position = D3DXVECTOR3(0.5f, 0, -0.5f);
	_Vertices[1].Color = Color;
	_Vertices[1].Normal = D3DXVECTOR3(0, -1, 0);
	_Vertices[1].UV = D3DXVECTOR2(1, 0);

	_Vertices[2].Position = D3DXVECTOR3(-0.5f, 0, 0.5f);
	_Vertices[2].Color = Color;
	_Vertices[2].Normal = D3DXVECTOR3(0, -1, 0);
	_Vertices[2].UV = D3DXVECTOR2(0, 1);

	_Vertices[3].Position = D3DXVECTOR3(0.5f, 0, 0.5f);
	_Vertices[3].Color = Color;
	_Vertices[3].Normal = D3DXVECTOR3(0, -1, 0);
	_Vertices[3].UV = D3DXVECTOR2(1, 1);

	// Spitze
	_Vertices[4].Position = D3DXVECTOR3(0, 1, 0);
	_Vertices[4].Color = Color;
	_Vertices[4].Normal = D3DXVECTOR3(0, 1, 0);
	_Vertices[4].UV = D3DXVECTOR2(0.5f, 0.5f);


	//Copy Data to Device (Map + UnMap)
	D3D11_MAPPED_SUBRESOURCE _VBMap;
	m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMap);

	// von _Vertices nach _VBMap.pData kopieren
	memcpy(_VBMap.pData, _Vertices, sizeof(Pyramid_Vertex) * 5);
	
	m_pDeviceContext->Unmap(m_pVertexBuffer, 0);

	// --------------------------------------------------------- IndexBuffer -----------------------------------------------------
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IBDesc.ByteWidth = sizeof(DWORD) * (6  * 3); // Größe von einem Element * Anzahl Elemente
	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);
	
	DWORD _Indices[18]; // (6 * 3) Indices

	// Boden
	_Indices[0] = 1;
	_Indices[1] = 0;
	_Indices[2] = 2;
	_Indices[3] = 1;
	_Indices[4] = 2;
	_Indices[5] = 3;
	
	// Vorne
	_Indices[6] = 0;
	_Indices[7] = 4;
	_Indices[8] = 1;
	
	// Rechts
	_Indices[9] = 1;
	_Indices[10] = 4;
	_Indices[11] = 3;
	
	// Hinten
	_Indices[12] = 3;
	_Indices[13] = 4;
	_Indices[14] = 2;
	
	// Links
	_Indices[15] = 0;
	_Indices[16] = 2;
	_Indices[17] = 4;


	//Copy Data to Device (Map + UnMap)
	D3D11_MAPPED_SUBRESOURCE _IBMap;
	m_pDeviceContext->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMap);

	// von _Indices nach _IBMap.pData kopieren
	memcpy(_IBMap.pData, _Indices, sizeof(DWORD) * 18);

	m_pDeviceContext->Unmap(m_pIndexBuffer, 0);





	// --------------------------------------------------------- VertexShader ----------------------------------------------------
	// --------------------------------------------------------- PixelShader -----------------------------------------------------
	// --------------------------------------------------------- InputLayout -----------------------------------------------------
	// --------------------------------------------------------- MatrixBuffer ----------------------------------------------------

}

void Pyramid::Update(float p_DeltaTime, Camera* p_pCamera)
{

}

void Pyramid::Draw()
{
	
}