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

	m_pTexture = new Texture("StoneTexture.jpg", m_pDevice);


	// --------------------------------------------------------- VertexBuffer ----------------------------------------------------


	// << 1. VertexBuffer erstellen >>
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBDesc.ByteWidth = sizeof(Pyramid_Vertex) * 5; // Größe von einem Element * Anzahl Elemente
	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);



	// << 2. VertexBuffer übertragen >>
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
	

	// << 3. IndexBuffer erstellen >>
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IBDesc.ByteWidth = sizeof(DWORD) * (6  * 3); // Größe von einem Element * Anzahl Elemente
	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);
	


	// << 4. IndexBuffer übertragen >>
	DWORD _Indices[18]; // (6 * 3) Indices

	// Boden
	_Indices[0] = 1;
	_Indices[1] = 2;
	_Indices[2] = 0;
	_Indices[3] = 1;
	_Indices[4] = 3;
	_Indices[5] = 2;
	
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


	// --------------------------------------------------------- PixelShader -----------------------------------------------------

	// << 5. Pixelshader laden >>
	ID3D10Blob* _pPixelBlob;
	ID3D10Blob* _pError;

	if (S_OK != D3DX11CompileFromFile("Pyramid.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr, &_pPixelBlob, &_pError, nullptr))
	{
		MessageBox(NULL, (char*)_pError->GetBufferPointer() , "Fehler im PixelShader", 0);
		PostQuitMessage(0);
		return;
	}


	m_pDevice->CreatePixelShader(_pPixelBlob->GetBufferPointer(), _pPixelBlob->GetBufferSize() , nullptr, &m_pPixelShader);


	// --------------------------------------------------------- VertexShader ----------------------------------------------------
	// << 6. VertexShader laden >>

	ID3D10Blob* _pVertexBlob;

	if (S_OK != D3DX11CompileFromFile("Pyramid.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr, &_pVertexBlob, &_pError, nullptr))
	{
		MessageBox(NULL, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", 0);
	}

	m_pDevice->CreateVertexShader(_pVertexBlob->GetBufferPointer(), _pVertexBlob->GetBufferSize(), nullptr, &m_pVertexShader);

	// --------------------------------------------------------- MatrixBuffer ----------------------------------------------------
	// << 7. ConstantBuffer erstellen >>
	D3D11_BUFFER_DESC _MatrixBufferDesc;
	ZeroMemory(&_MatrixBufferDesc, sizeof(_MatrixBufferDesc));
	
	_MatrixBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MatrixBufferDesc.ByteWidth = sizeof(Pyramid_MatrixBuffer);
	_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	//_MatrixBufferDesc.MiscFlags = 0;
	//_MatrixBufferDesc.StructureByteStride = 0;
	_MatrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_MatrixBufferDesc, nullptr, &m_pConstantBuffer);

	//--------------------------------------------------------- InputLayout -----------------------------------------------------

	// << 8. InputLayout Erstellen >>
	D3D11_INPUT_ELEMENT_DESC _IED[4];
	// Position
	_IED[0].InputSlot = 0;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;

	// Normal
	_IED[1].InputSlot = 0;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].AlignedByteOffset = sizeof(float) * 3;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[1].SemanticName = "NORMAL";
	_IED[1].SemanticIndex = 0;

	// Color
	_IED[2].InputSlot = 0;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].AlignedByteOffset = sizeof(float) * 6;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[2].SemanticName = "COLOR";
	_IED[2].SemanticIndex = 0;

	// UV
	_IED[3].InputSlot = 0;
	_IED[3].InstanceDataStepRate = 0;
	_IED[3].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[3].AlignedByteOffset = sizeof(float) * 9;
	_IED[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[3].SemanticName = "TEXCOORD";
	_IED[3].SemanticIndex = 0;

	m_pDevice->CreateInputLayout(_IED, 4, _pVertexBlob->GetBufferPointer(), _pVertexBlob->GetBufferSize(), &m_pInputLayout);
}

void Pyramid::Update(float p_DeltaTime, Camera* p_pCamera)
{
	// << 9. ConstantBuffer übertragen >>
	Pyramid_MatrixBuffer _ConstantBuffer;
	_ConstantBuffer.WorldViewProjectionMatrix = p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();

	D3D11_MAPPED_SUBRESOURCE _MappedCB;

	m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MappedCB);

	memcpy(_MappedCB.pData, &_ConstantBuffer, sizeof(_ConstantBuffer));

	m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
}

void Pyramid::Draw()
{
	UINT stride = sizeof(Pyramid_Vertex);
	UINT offset = 0;

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);

	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pDeviceContext->PSSetShaderResources(0, 1, &(m_pTexture->m_pSRV));

	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	m_pDeviceContext->DrawIndexed(18, 0, 0);
}