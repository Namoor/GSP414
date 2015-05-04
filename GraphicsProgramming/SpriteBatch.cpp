#include "SpriteBatch.h"
#include "SpriteBatch_Structs.h"




SpriteBatch::SpriteBatch()
{
	m_pDevice = nullptr;
	m_pDevCon = nullptr;

	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;

	m_pInputLayout = nullptr;
}

void SpriteBatch::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDeviceContext;


	// VertexBuffer -----------------------------------------
	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.ByteWidth = 1000 * 4 * sizeof(SpriteBatch_Vertex); // Platz für 1000 Sprites


	m_pDevice->CreateBuffer(&_VBufferDesc, nullptr, &m_pVertexBuffer);


	// IndexBuffer --------------------------------------------
	D3D11_BUFFER_DESC _IBufferDesc;
	ZeroMemory(&_IBufferDesc, sizeof(_IBufferDesc));

	_IBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBufferDesc.ByteWidth = 1000 * 6 * sizeof(DWORD); // Platz für 1000 Sprites


	m_pDevice->CreateBuffer(&_IBufferDesc, nullptr, &m_pIndexBuffer);


	// Shader -------------------------------------------------
	ID3D10Blob* _pError;

	// Pixelshader
	ID3D10Blob* _pPixelBlob;
	if (D3DX11CompileFromFile(TEXT("SpriteBatchShader.hlsl"), nullptr, nullptr, TEXT("PShader"),
		TEXT("ps_5_0"), 0, 0, nullptr, &_pPixelBlob, &_pError, nullptr) != S_OK)
	{
		// Shadercompilation fehlgeschlagen

		MessageBox(0, (char*)_pError->GetBufferPointer(), TEXT("Fehler im Pixelshader vom SpriteBatch"), 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelBlob->GetBufferPointer(), _pPixelBlob->GetBufferSize(), nullptr, &m_pPixelShader);

	// VertexShader
	ID3D10Blob* _pVertexBlob;
	if (D3DX11CompileFromFile(TEXT("SpriteBatchShader.hlsl"), nullptr, nullptr, TEXT("VShader"),
		TEXT("vs_5_0"), 0, 0, nullptr, &_pVertexBlob, &_pError, nullptr) != S_OK)
	{
		// Shadercompilation fehlgeschlagen

		MessageBox(0, (char*)_pError->GetBufferPointer(), TEXT("Fehler im VertexShader vom SpriteBatch"), 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexBlob->GetBufferPointer(), _pVertexBlob->GetBufferSize(), nullptr, &m_pVertexShader);




	// InputLayout --------------------------------------------

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

void SpriteBatch::DrawTexture(Rect p_DestinationRectangle, Texture* p_pTexture, Rect p_SourceRectangle, D3DXCOLOR p_Color)
{
	// Zeichnet nicht direkt die Texture, sondern speichert sie im Commandbuffer

	DrawCommand _NewCommand = { p_DestinationRectangle, p_pTexture, p_SourceRectangle, p_Color };


	m_DrawCommands.push_back(_NewCommand);
}

void SpriteBatch::Begin()
{
	m_DrawCommands.clear();
}

void SpriteBatch::End()
{
	// Drawcommands abarbeiten und zeichnen
}