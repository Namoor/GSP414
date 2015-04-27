#include "Simple2DObject.h"

#include "Simple2DObject_Structs.h"



Simple2DObject::Simple2DObject()
{
	m_pDevice = nullptr;
	m_pDevCon = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;

	m_pInputLayout = nullptr;
}

Simple2DObject::~Simple2DObject()
{

}


void Simple2DObject::Initialize(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;


	m_pTexture = new Texture("Tex.tga", m_pDevice);

	// Initializieren des Rechteckes


	// VertexBuffer ---------------------------------------------------------


	S2DO_Vertices _Vertices[4];

	_Vertices[0].Position = D3DXVECTOR3(-0.6f, -0.6f, 0);
	_Vertices[0].Color = D3DXVECTOR4(1, 0, 0, 1);
	_Vertices[0].UV = D3DXVECTOR2(0, 0);

	_Vertices[1].Position = D3DXVECTOR3(0.6f, -0.6f, 0);
	_Vertices[1].Color = D3DXVECTOR4(1, 1, 0, 1);
	_Vertices[1].UV = D3DXVECTOR2(1, 0);

	_Vertices[2].Position = D3DXVECTOR3(-0.6f, 0.6f, 0);
	_Vertices[2].Color = D3DXVECTOR4(1, 0, 0, 1);
	_Vertices[2].UV = D3DXVECTOR2(0, 1);

	_Vertices[3].Position = D3DXVECTOR3(0.6f, 0.6f, 0);
	_Vertices[3].Color = D3DXVECTOR4(1, 0, 0, 1);
	_Vertices[3].UV = D3DXVECTOR2(1, 1);


	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.ByteWidth = 4 * sizeof(S2DO_Vertices); // Anzahl an Vertices(4) * Größe einer Vertice(36)


	m_pDevice->CreateBuffer(&_VBufferDesc, nullptr , &m_pVertexBuffer);

	
	D3D11_MAPPED_SUBRESOURCE _VBufferRes;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBufferRes);
	
	// Daten manipulieren
	memcpy(_VBufferRes.pData, &_Vertices, 4 * sizeof(S2DO_Vertices));

	m_pDevCon->Unmap(m_pVertexBuffer, 0);



	// IndexBuffer -------------------------------------------------------------------

	DWORD _Indices[6];
	_Indices[0] = 0;
	_Indices[1] = 2;
	_Indices[2] = 1;
	_Indices[3] = 1;
	_Indices[4] = 2;
	_Indices[5] = 3;


	D3D11_BUFFER_DESC _IBufferDesc;
	ZeroMemory(&_IBufferDesc, sizeof(_IBufferDesc));

	_IBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBufferDesc.ByteWidth = 6 * sizeof(DWORD); // Anzahl an Indices(6) * Größe eines Indices(4)


	m_pDevice->CreateBuffer(&_IBufferDesc, nullptr, &m_pIndexBuffer);


	D3D11_MAPPED_SUBRESOURCE _IBufferRes;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBufferRes);

	// Daten manipulieren
	memcpy(_IBufferRes.pData, &_Indices, 6 * sizeof(DWORD));

	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	// Shader ---------------------------------------------------------------------------

	ID3D10Blob* _pError;

	// Pixelshader
	ID3D10Blob* _pPixelBlob;
	if (D3DX11CompileFromFile(TEXT("Simple2DShader.hlsl"), nullptr, nullptr, TEXT("PShader"),
		TEXT("ps_5_0"), 0, 0, nullptr, &_pPixelBlob, &_pError, nullptr) != S_OK)
	{
		// Shadercompilation fehlgeschlagen

		MessageBox(0, (char*)_pError->GetBufferPointer(), TEXT("Fehler im Pixelshader"), 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelBlob->GetBufferPointer(), _pPixelBlob->GetBufferSize(), nullptr, &m_pPixelShader);

	// VertexShader
	ID3D10Blob* _pVertexBlob;
	if (D3DX11CompileFromFile(TEXT("Simple2DShader.hlsl"), nullptr, nullptr, TEXT("VShader"),
		TEXT("vs_5_0"), 0, 0, nullptr, &_pVertexBlob, &_pError, nullptr) != S_OK)
	{
		// Shadercompilation fehlgeschlagen

		MessageBox(0, (char*)_pError->GetBufferPointer(), TEXT("Fehler im VertexShader"), 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexBlob->GetBufferPointer(), _pVertexBlob->GetBufferSize(), nullptr, &m_pVertexShader);



	// ------------------------------------------------------------ Input Layout -------------------------------------
	D3D11_INPUT_ELEMENT_DESC _InputElementDesc[3];

	//LPCSTR SemanticName;
	//UINT SemanticIndex;
	//DXGI_FORMAT Format;
	//UINT InputSlot;
	//UINT AlignedByteOffset;
	//D3D11_INPUT_CLASSIFICATION InputSlotClass;
	//UINT InstanceDataStepRate;

	_InputElementDesc[0].SemanticName = "POSITION";
	_InputElementDesc[0].SemanticIndex = 0;
	_InputElementDesc[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_InputElementDesc[0].InputSlot = 0;
	_InputElementDesc[0].AlignedByteOffset = 0;
	_InputElementDesc[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_InputElementDesc[0].InstanceDataStepRate = 0;

	_InputElementDesc[1].SemanticName = "COLOR";
	_InputElementDesc[1].SemanticIndex = 0;
	_InputElementDesc[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_InputElementDesc[1].InputSlot = 0;
	_InputElementDesc[1].AlignedByteOffset = 12;
	_InputElementDesc[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_InputElementDesc[1].InstanceDataStepRate = 0;

	_InputElementDesc[2].SemanticName = "TEXCOORD";
	_InputElementDesc[2].SemanticIndex = 0;
	_InputElementDesc[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_InputElementDesc[2].InputSlot = 0;
	_InputElementDesc[2].AlignedByteOffset = 28;
	_InputElementDesc[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_InputElementDesc[2].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_InputElementDesc, 3, _pVertexBlob->GetBufferPointer(),
		_pVertexBlob->GetBufferSize(), &m_pInputLayout);
}

void Simple2DObject::Draw()
{
	UINT stride = sizeof(S2DO_Vertices);
	UINT offset = 0;

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	// Input Assembler
	m_pDevCon->IASetInputLayout(m_pInputLayout);

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT , 0);

	m_pDevCon->PSSetShaderResources(0, 1, &(m_pTexture->m_pSRV));

	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->DrawIndexed(6, 0, 0);
}


