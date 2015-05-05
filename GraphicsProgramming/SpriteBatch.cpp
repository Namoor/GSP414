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


	// VertexBuffer -----------------------------------------------------------------------

	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.ByteWidth = sizeof(SpriteBatch_Vertex) * 4 * 1000;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);


	// IndexBuffer -----------------------------------------------------------------------

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.ByteWidth = sizeof(DWORD) * (3 * 2) * 1000;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	// VertexShader ----------------------------------------------------------------

	ID3D10Blob* _pVertexShaderBlob;
	ID3D10Blob* _pErrorBlob;

	if (D3DX11CompileFromFile(TEXT("SpriteBatchShader.hlsl"), nullptr, nullptr, TEXT("VShader"),
		TEXT("vs_5_0"), 0, 0, nullptr, &_pVertexShaderBlob, &_pErrorBlob, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pErrorBlob->GetBufferPointer(), TEXT("Fehler im VertexShader"), 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexShaderBlob->GetBufferPointer(),
		_pVertexShaderBlob->GetBufferSize() , nullptr, &m_pVertexShader);

	// PixelShader ----------------------------------------------------------------

	ID3D10Blob* _pPixelShaderBlob;

	if (D3DX11CompileFromFile(TEXT("SpriteBatchShader.hlsl"), nullptr, nullptr, TEXT("PShader"),
		TEXT("ps_5_0"), 0, 0, nullptr, &_pPixelShaderBlob, &_pErrorBlob, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pErrorBlob->GetBufferPointer(), TEXT("Fehler im PixelShader"), 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShaderBlob->GetBufferPointer(),
		_pPixelShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);


	// InputElementDescription ------------------------------------------------------------

	D3D11_INPUT_ELEMENT_DESC _IED[3];

	// VertexPosition
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;

	// VertexColor
	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;

	// VertexUVData
	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 28;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_IED, 3,
		_pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(),
		&m_pInputLayout);
}

void SpriteBatch::DrawTexture(Rect p_DestinationRectangle, Texture* p_pTexture, Rect p_SourceRectangle, D3DXVECTOR4 p_Color)
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

	int _SpriteCount = m_DrawCommands.size();

	int* _pIndices;
	SpriteBatch_Vertex* _pVertices;

	_pIndices = new int[_SpriteCount * 6];
	_pVertices = new SpriteBatch_Vertex[_SpriteCount * 4];

	int CurrentIndex = 0; // Speichert welcher SPrite gerade bearbeitet wird

	for (std::list<DrawCommand>::iterator _It = m_DrawCommands.begin(); _It != m_DrawCommands.end(); _It++)
	{
		// Sprite Bearbeiten
		DrawCommand _Command = *_It;

		float X = _Command.m_DestinationRectangle.x;
		float Y = _Command.m_DestinationRectangle.y;
		float Width = _Command.m_DestinationRectangle.width;
		float Height = _Command.m_DestinationRectangle.height;

		float UV_X = _Command.m_SourceRectangle.x;
		float UV_Y = _Command.m_SourceRectangle.y;
		float UV_Width = _Command.m_SourceRectangle.width;
		float UV_Height = _Command.m_SourceRectangle.height;
		
		// Color
		_pVertices[CurrentIndex * 4 + 0].Color = _Command.m_Color;
		_pVertices[CurrentIndex * 4 + 1].Color = _Command.m_Color;
		_pVertices[CurrentIndex * 4 + 2].Color = _Command.m_Color;
		_pVertices[CurrentIndex * 4 + 3].Color = _Command.m_Color;

		// Position
		_pVertices[CurrentIndex * 4 + 0].Position = D3DXVECTOR3(X, 0, Y);
		_pVertices[CurrentIndex * 4 + 1].Position = D3DXVECTOR3(X + Width, 0, Y);
		_pVertices[CurrentIndex * 4 + 2].Position = D3DXVECTOR3(X, 0, Y + Height);
		_pVertices[CurrentIndex * 4 + 3].Position = D3DXVECTOR3(X + Width, 0, Y + Height);

		// UV
		_pVertices[CurrentIndex * 4 + 0].UV = D3DXVECTOR2(UV_X, UV_Y);
		_pVertices[CurrentIndex * 4 + 1].UV = D3DXVECTOR2(UV_X + UV_Width, UV_Y);
		_pVertices[CurrentIndex * 4 + 2].UV = D3DXVECTOR2(UV_X, UV_Y + UV_Height);
		_pVertices[CurrentIndex * 4 + 3].UV = D3DXVECTOR2(UV_X + UV_Width, UV_Y + UV_Height);




		CurrentIndex++;
	}


}



