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

	m_pTexture = new Texture("StoneTexture.jpg", p_pDevice);

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
			D3DXVECTOR3 Position = D3DXVECTOR3(x, GetHeightAt(x, y), y);
			D3DXVECTOR3 Position0n = D3DXVECTOR3(x, GetHeightAt(x, y - 1), y - 1);
			D3DXVECTOR3 Position0p = D3DXVECTOR3(x, GetHeightAt(x, y + 1), y + 1);
			D3DXVECTOR3 Positionn0 = D3DXVECTOR3(x - 1, GetHeightAt(x - 1, y), y);
			D3DXVECTOR3 Positionp0 = D3DXVECTOR3(x + 1, GetHeightAt(x + 1, y), y);

			D3DXVECTOR3 Dir0n = Position0n - Position;
			D3DXVECTOR3 Dir0p = Position0p - Position;
			D3DXVECTOR3 Dirn0 = Positionn0 - Position;
			D3DXVECTOR3 Dirp0 = Positionp0 - Position;

			D3DXVec3Normalize(&Dir0n, &Dir0n);
			D3DXVec3Normalize(&Dir0p, &Dir0p);
			D3DXVec3Normalize(&Dirn0, &Dirn0);
			D3DXVec3Normalize(&Dirp0, &Dirp0);

			D3DXVECTOR3 Normal = D3DXVECTOR3(0,0,0);


			D3DXVECTOR3 Temp;
			D3DXVec3Cross(&Temp, &Dir0n, &Dirp0);
			Normal += Temp;
			D3DXVec3Cross(&Temp, &Dirp0, &Dir0p);
			Normal += Temp;
			D3DXVec3Cross(&Temp, &Dir0p, &Dirn0);
			Normal += Temp;
			D3DXVec3Cross(&Temp, &Dirn0, &Dir0n);
			Normal += Temp;


			D3DXVec3Normalize(&(_Array[y * (p_XSize + 1) + x].m_Normal), &Normal);

			_Array[y * (p_XSize + 1) + x].m_Color = D3DXVECTOR4(1, 1, 1, 1);
			_Array[y * (p_XSize + 1) + x].m_Position = Position;
			_Array[y * (p_XSize + 1) + x].m_UV = D3DXVECTOR2(x, y);



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

	D3D11_INPUT_ELEMENT_DESC _IED[4];
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
	_IED[2].AlignedByteOffset = 28;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;

	_IED[3].AlignedByteOffset = 36;
	_IED[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[3].InputSlot = 0;
	_IED[3].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[3].InstanceDataStepRate = 0;
	_IED[3].SemanticName = "NORMAL";
	_IED[3].SemanticIndex = 0;


	m_pDevice->CreateInputLayout(_IED, 4, _pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), &m_pInputLayout);

	// ------------------------------------- Constant Buffer --------------------------------------------------------------------

	D3D11_BUFFER_DESC _MatrixBufferDesc;
	ZeroMemory(&_MatrixBufferDesc, sizeof(_MatrixBufferDesc));

	_MatrixBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MatrixBufferDesc.ByteWidth = 64;
	_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MatrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;


	m_pDevice->CreateBuffer(&_MatrixBufferDesc, nullptr, &m_pMatrixConstantBuffer);

	// ----------------------------------------- Sampler State ------------------------------------------------------------------

	D3D11_SAMPLER_DESC _SamplerStateDesc;
	ZeroMemory(&_SamplerStateDesc, sizeof(_SamplerStateDesc));

	_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	_SamplerStateDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;


	_SamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	_SamplerStateDesc.MaxAnisotropy = 16;

	_SamplerStateDesc.MinLOD = 0;
	_SamplerStateDesc.MaxLOD = 10;

	_SamplerStateDesc.MipLODBias = -2;

	m_pDevice->CreateSamplerState(&_SamplerStateDesc, &m_pTextureSampler);



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

	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture->m_pSRV);
	m_pDevCon->PSSetSamplers(0, 1, &m_pTextureSampler);

	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);



	m_pDevCon->DrawIndexed(IndexCount, 0, 0);
}


float Plane::GetHeightAt(int x, int y)
{
	return sin(((float)x) / 2) * 4 + sin(((float)y) / 2) * 4;
}
