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


	IndexCount = (p_XSize*p_YSize * 6);

	// ----------------------------- VertexBuffer -------------------------------------------------

	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
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
			_Array[y * (p_XSize + 1) + x].m_Position = D3DXVECTOR3(x, GetHeightAt(x,y), y);

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


	D3D11_MAPPED_SUBRESOURCE _VertexBufferMapped;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VertexBufferMapped);

	memcpy(_VertexBufferMapped.pData, _Array, ((p_XSize + 1) * (p_YSize + 1)) * sizeof(Plane_Vertex));

	m_pDevCon->Unmap(m_pVertexBuffer, 0);



	// ------------------------------------ IndexBuffer -----------------------------------------------

	D3D11_BUFFER_DESC _IndexBufferDesc;
	ZeroMemory(&_IndexBufferDesc, sizeof(_IndexBufferDesc));

	_IndexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IndexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IndexBufferDesc.ByteWidth = (p_XSize*p_YSize * 6)*sizeof(DWORD);


	m_pDevice->CreateBuffer(&_IndexBufferDesc, nullptr, &m_pIndexBuffer);


	DWORD* _Indices = new DWORD[p_XSize*p_YSize * 6];

	for (int y = 0; y < p_YSize; y++)
	{
		for (int x = 0; x < p_XSize; x++)
		{
			int _CurrentQuad = p_XSize * y + x;

			int UntenLinks = y * (p_XSize + 1) + x;
			int UntenRechts = UntenLinks + 1;
			int ObenLinks = UntenLinks + (p_XSize + 1);
			int ObenRechts = ObenLinks + 1;

			_Indices[_CurrentQuad * 6 + 0] = UntenLinks;
			_Indices[_CurrentQuad * 6 + 1] = ObenLinks;
			_Indices[_CurrentQuad * 6 + 2] = UntenRechts;
			_Indices[_CurrentQuad * 6 + 3] = UntenRechts;
			_Indices[_CurrentQuad * 6 + 4] = ObenLinks;
			_Indices[_CurrentQuad * 6 + 5] = ObenRechts;
		}
	}

	D3D11_MAPPED_SUBRESOURCE _IndexBufferMapped;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IndexBufferMapped);

	memcpy(_IndexBufferMapped.pData, _Indices, (p_XSize*p_YSize * 6)*sizeof(DWORD));

	m_pDevCon->Unmap(m_pIndexBuffer, 0);

	// ----------------------- Shader ----------------------------------------------------------------------------------

	ID3D10Blob* _pError;
	ID3D10Blob* _pVertexShader;

	HRESULT HRES = D3DX11CompileFromFile(TEXT("Plane.hlsl"), nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr, &_pVertexShader, &_pError, nullptr);

	if (HRES != S_OK)
	{
		// Fehlerbehandlung
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", MB_OK);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);

	// ------------------------------------- PixelShader --------------------------------------------------

	ID3D10Blob* _pPixelShader;

	HRES = D3DX11CompileFromFile(TEXT("Plane.hlsl"), nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr, &_pPixelShader, &_pError, nullptr);

	if (HRES != S_OK)
	{
		// Fehlerbehandlung
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", MB_OK);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);


	// ----------------------------------------- InputLayout -----------------------------------------------------------

	D3D11_INPUT_ELEMENT_DESC _IED[2];
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;


	m_pDevice->CreateInputLayout(_IED, 2, _pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), &m_pInputLayout);

	// ------------------------------------- Constant Buffer --------------------------------------------------------------------

	D3D11_BUFFER_DESC _MatrixBufferDesc;
	ZeroMemory(&_MatrixBufferDesc, sizeof(_MatrixBufferDesc));

	_MatrixBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MatrixBufferDesc.ByteWidth = 64;
	_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MatrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;


	m_pDevice->CreateBuffer(&_MatrixBufferDesc, nullptr, &m_pMatrixConstantBuffer);
}

void Plane::Update(float DeltaTime, Camera* p_pCamera)
{
	D3DXMATRIX _MatrixData = p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();

	D3D11_MAPPED_SUBRESOURCE _MatrixMapped;

	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MatrixMapped);

	memcpy(_MatrixMapped.pData, &_MatrixData, 64);

	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);
}

void Plane::Draw()
{
	UINT _Offset = 0;
	UINT _Stride = sizeof(Plane_Vertex);

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);



	m_pDevCon->DrawIndexed(IndexCount, 0, 0);
}


float Plane::GetHeightAt(int x, int y)
{
	return sin(((float)x) / 5);
}
