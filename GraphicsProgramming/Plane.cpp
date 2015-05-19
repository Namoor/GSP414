#include "Plane.h"

#include "Plane_Structs.h"



Plane::Plane()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;

	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;

	m_pInputLayout = nullptr;

	m_pMatrixConstantBuffer = nullptr;
}

Plane::~Plane()
{
	if (m_pIndexBuffer != nullptr)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}
}

void Plane::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_XSize, int p_YSize)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;


	// ----------------------------- VertexBuffer -------------------------------------------------

	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	_VBufferDesc.ByteWidth = ((p_XSize + 1) * (p_YSize + 1)) * sizeof(Plane_Vertex); // Anzahl an Objekten (In diesem Fall Vertices) * Größe von eínem Objekt

	m_pDevice->CreateBuffer(&_VBufferDesc, nullptr, &m_pVertexBuffer);


	Plane_Vertex* _Array = new Plane_Vertex[(p_XSize + 1) * (p_YSize + 1)];

	//int i = 0;

	// Mit 2 Forschleifen durch die Plane wandern und Vertices erstellen
	for (int y = 0; y <= p_YSize; y++)
	{
		for (int x = 0; x <= p_XSize; x++)
		{
			_Array[y * (p_XSize + 1) + x].m_Color = D3DXVECTOR4(1, 1, 1, 1);
			_Array[y * (p_XSize + 1) + x].m_Position = D3DXVECTOR3(x, 0, y);

		}
	}

	//int i = 0;
	//for (int y = 0; y <= p_YSize; y++)
	//{
	//	for (int x = 0; x <= p_XSize; x++)
	//	{
	//		_Array[i].m_Color = D3DXVECTOR4(1, 1, 1, 1);
	//		_Array[i].m_Position = D3DXVECTOR3(x, 0, y);
	//		i++;
	//	}
	//}


}

void Plane::Update(float DeltaTime)
{

}

void Plane::Draw()
{

}

