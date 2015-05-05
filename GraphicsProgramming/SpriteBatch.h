#include <list>

#include "d3dinclude.h"

#include "Rect.h"
#include "Texture.h"

struct DrawCommand
{
	Rect m_DestinationRectangle;
	Texture* m_pTexture;
	Rect m_SourceRectangle;
	D3DXVECTOR4 m_Color;
};

class SpriteBatch
{

public:

	SpriteBatch();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext);

	void DrawTexture(Rect p_DestinationRectangle, Texture* p_pTexture, Rect p_SourceRectangle, D3DXVECTOR4 p_Color);

	void Begin();
	void End();

private:

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11InputLayout* m_pInputLayout;

	std::list<DrawCommand> m_DrawCommands;
};