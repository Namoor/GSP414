#include "2DScene.h"
#include "Texture.h"


Scene2D::Scene2D()
{
	m_pFirstObject = nullptr;
	m_pSpriteBatch = nullptr;
}

Scene2D::~Scene2D()
{

}

void Scene2D::Initialize(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;

	m_pFirstObject = new Simple2DObject();
	m_pFirstObject->Initialize(p_pDevice, p_pDevCon);

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(p_pDevice, p_pDevCon);

	m_pTexture1 = new Texture("Coin.png", m_pDevice);
}

void Scene2D::Update(float p_DeltaTime)
{

	m_pSpriteBatch->Begin();

	m_pSpriteBatch->DrawTexture(Rect(0, 0, 0.5f, 0.5f), m_pTexture1, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawTexture(Rect(0, 0.5f, 0.5f, 0.5f), m_pTexture1, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawTexture(Rect(0.5f, 0, 0.5f, 0.5f), m_pTexture1, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));


	m_pSpriteBatch->End();
}

void Scene2D::Render()
{
	m_pFirstObject->Draw();
}