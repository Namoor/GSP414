#include "TilingTerrain.h"

#include "TilingTerrain_Structs.h"



TilingTerrain::TilingTerrain()
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

TilingTerrain::~TilingTerrain()
{
	if (m_pIndexBuffer != nullptr)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}
}

void TilingTerrain::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_XSize, int p_YSize, TilingTerrain_Data* p_pData)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	m_pGrassTexture = new Texture("TextureAtlas.png", p_pDevice);


	int _TotalFaceCount = p_XSize * p_YSize;
	int _TotalVertexCount = _TotalFaceCount * 4;

	IndexCount = _TotalFaceCount * 2 * 3;

	// ----------------------------- VertexBuffer -------------------------------------------------

	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBufferDesc.ByteWidth = _TotalVertexCount * sizeof(TilingTerrain_Vertex); // Anzahl an Objekten (In diesem Fall Vertices) * Größe von eínem Objekt

	m_pDevice->CreateBuffer(&_VBufferDesc, nullptr, &m_pVertexBuffer);


	TilingTerrain_Vertex* _Array = new TilingTerrain_Vertex[_TotalVertexCount];

	//int i = 0;

	// Mit 2 Forschleifen durch die Plane wandern und Vertices erstellen
	for (int y = 0; y < p_YSize; y++)
	{
		for (int x = 0; x < p_XSize; x++)
		{
			int _FaceIndex = y * p_XSize + x;

			float Offset = 0.5 / 512.0f;

			TilingTerrainType _UntenLinks = p_pData->GetData(x, y);
			TilingTerrainType _UntenRechts = p_pData->GetData(x + 1, y);
			TilingTerrainType _ObenLinks = p_pData->GetData(x, y + 1);
			TilingTerrainType _ObenRechts = p_pData->GetData(x + 1, y + 1);

			int TexCoordX = 4;
			int TexCoordY = 4;

			int _TexCoordID = 0;       // 0000
			if (_UntenRechts == TilingTerrainType::Grass) _TexCoordID |= 1; // 0001
			if (_UntenLinks == TilingTerrainType::Grass) _TexCoordID |= 2; // 0010
			if (_ObenRechts == TilingTerrainType::Grass) _TexCoordID |= 4; // 0100
			if (_ObenLinks == TilingTerrainType::Grass) _TexCoordID |= 8; // 1000


			TexCoordX = _TexCoordID % 4;
			TexCoordY = _TexCoordID / 4;
			
			
			if (_TexCoordID == 15)
			{
				int _Rnd = rand() % 101;

				if (_Rnd  < 70)
				{
					// Eines der Random Tiles auswählen
					int _RandomTileID = rand() % 8;

					if (_Rnd < 15 && _Rnd > 5)
						_RandomTileID = rand() % 4 + 8;
					if (_Rnd < 5 )
						_RandomTileID = rand() % 4 + 12;

					TexCoordX = 4 + _RandomTileID % 4;
					TexCoordY = _RandomTileID / 4;
				}
			}



			// Unten Links
			_Array[_FaceIndex * 4 + 0].m_Normal = D3DXVECTOR3(0, 1, 0);
			_Array[_FaceIndex * 4 + 0].m_UV = D3DXVECTOR2(TexCoordX * 0.125f + Offset, TexCoordY * 0.125f + 0.125f - Offset);
			_Array[_FaceIndex * 4 + 0].m_Position = D3DXVECTOR3(x, 0, y);

			// Unten Rechts
			_Array[_FaceIndex * 4 + 1].m_Normal = D3DXVECTOR3(0, 1, 0);
			_Array[_FaceIndex * 4 + 1].m_UV = D3DXVECTOR2(TexCoordX * 0.125f + 0.125f - Offset, TexCoordY * 0.125f + 0.125f - Offset);
			_Array[_FaceIndex * 4 + 1].m_Position = D3DXVECTOR3(x + 1, 0, y);

			// ObenLinks
			_Array[_FaceIndex * 4 + 2].m_Normal = D3DXVECTOR3(0, 1, 0);
			_Array[_FaceIndex * 4 + 2].m_UV = D3DXVECTOR2(TexCoordX * 0.125f + Offset, TexCoordY * 0.125f + Offset);
			_Array[_FaceIndex * 4 + 2].m_Position = D3DXVECTOR3(x, 0, y + 1);

			// ObenRechts
			_Array[_FaceIndex * 4 + 3].m_Normal = D3DXVECTOR3(0, 1, 0);
			_Array[_FaceIndex * 4 + 3].m_UV = D3DXVECTOR2(TexCoordX * 0.125f + 0.125f - Offset, TexCoordY * 0.125f + Offset);
			_Array[_FaceIndex * 4 + 3].m_Position = D3DXVECTOR3(x + 1, 0, y + 1);
		}
	}

	D3D11_MAPPED_SUBRESOURCE _VertexBufferMapped;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VertexBufferMapped);

	memcpy(_VertexBufferMapped.pData, _Array, (_TotalVertexCount)* sizeof(TilingTerrain_Vertex));

	m_pDevCon->Unmap(m_pVertexBuffer, 0);



	// ------------------------------------ IndexBuffer -----------------------------------------------

	D3D11_BUFFER_DESC _IndexBufferDesc;
	ZeroMemory(&_IndexBufferDesc, sizeof(_IndexBufferDesc));

	_IndexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IndexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IndexBufferDesc.ByteWidth = IndexCount*sizeof(DWORD);


	m_pDevice->CreateBuffer(&_IndexBufferDesc, nullptr, &m_pIndexBuffer);


	DWORD* _Indices = new DWORD[IndexCount];

	for (int CurrentFace = 0; CurrentFace < _TotalFaceCount; CurrentFace++)
	{
		// Erste Triangle
		_Indices[CurrentFace * 6 + 0] = CurrentFace * 4 + 0;
		_Indices[CurrentFace * 6 + 1] = CurrentFace * 4 + 2;
		_Indices[CurrentFace * 6 + 2] = CurrentFace * 4 + 1;

		// Zweite Triangle
		_Indices[CurrentFace * 6 + 3] = CurrentFace * 4 + 1;
		_Indices[CurrentFace * 6 + 4] = CurrentFace * 4 + 2;
		_Indices[CurrentFace * 6 + 5] = CurrentFace * 4 + 3;
	}


	D3D11_MAPPED_SUBRESOURCE _IndexBufferMapped;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IndexBufferMapped);

	memcpy(_IndexBufferMapped.pData, _Indices, (IndexCount)*sizeof(DWORD));

	m_pDevCon->Unmap(m_pIndexBuffer, 0);



	// ----------------------- Shader ----------------------------------------------------------------------------------

	ID3D10Blob* _pError;
	ID3D10Blob* _pVertexShader;

	HRESULT HRES = D3DX11CompileFromFile(TEXT("TilingTerrain.hlsl"), nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr, &_pVertexShader, &_pError, nullptr);

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

	HRES = D3DX11CompileFromFile(TEXT("TilingTerrain.hlsl"), nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr, &_pPixelShader, &_pError, nullptr);

	if (HRES != S_OK)
	{
		// Fehlerbehandlung
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", MB_OK);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);






	// ----------------------------------------- InputLayout -----------------------------------------------------------

	D3D11_INPUT_ELEMENT_DESC _IED[3];
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;

	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].SemanticName = "NORMAL";
	_IED[1].SemanticIndex = 0;

	_IED[2].AlignedByteOffset = 24;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;


	m_pDevice->CreateInputLayout(_IED, 3, _pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), &m_pInputLayout);

	// ------------------------------------- Constant Buffer --------------------------------------------------------------------

	D3D11_BUFFER_DESC _MatrixBufferDesc;
	ZeroMemory(&_MatrixBufferDesc, sizeof(_MatrixBufferDesc));

	_MatrixBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MatrixBufferDesc.ByteWidth = 64;
	_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MatrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;


	m_pDevice->CreateBuffer(&_MatrixBufferDesc, nullptr, &m_pMatrixConstantBuffer);


	D3D11_BUFFER_DESC _PixelBufferDesc;
	ZeroMemory(&_PixelBufferDesc, sizeof(_PixelBufferDesc));

	_PixelBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_PixelBufferDesc.ByteWidth = 16;
	_PixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_PixelBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;


	m_pDevice->CreateBuffer(&_PixelBufferDesc, nullptr, &m_pPixelConstantBuffer);

	// ----------------------------------------- Sampler State ------------------------------------------------------------------

	D3D11_SAMPLER_DESC _SamplerStateDesc;
	ZeroMemory(&_SamplerStateDesc, sizeof(_SamplerStateDesc));

	_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	_SamplerStateDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;


	_SamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	_SamplerStateDesc.MaxAnisotropy = 0;

	_SamplerStateDesc.MinLOD = 0;
	_SamplerStateDesc.MaxLOD = 0;

	_SamplerStateDesc.MipLODBias = 0;

	m_pDevice->CreateSamplerState(&_SamplerStateDesc, &m_pTextureSampler);



}

void TilingTerrain::Update(float DeltaTime, Camera* p_pCamera)
{
	D3DXMATRIX WorldMatrix;
	D3DXMatrixIdentity(&WorldMatrix);

	D3DXMATRIX _MatrixData = WorldMatrix * p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();

	D3D11_MAPPED_SUBRESOURCE _MatrixMapped;

	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MatrixMapped);

	memcpy(_MatrixMapped.pData, &_MatrixData, 64);

	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);
}

void TilingTerrain::Draw()
{
	UINT _Offset = 0;
	UINT _Stride = sizeof(TilingTerrain_Vertex);

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDevCon->PSSetShaderResources(0, 1, &m_pGrassTexture->m_pSRV);



	m_pDevCon->PSSetSamplers(0, 1, &m_pTextureSampler);

	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);



	m_pDevCon->DrawIndexed(IndexCount, 0, 0);
}
