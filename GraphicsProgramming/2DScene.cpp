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
	m_pTexture2 = new Texture("SpriteSheet.png", m_pDevice);
}

void Scene2D::Update(float p_DeltaTime)
{

}

void Scene2D::Render()
{
	//m_pFirstObject->Draw();
	m_pSpriteBatch->Begin();

	/*for (int x = 0; x < 1000; x++)
	{
		int xPos = rand() % 1000;
		int yPos = rand() % 1000;
		m_pSpriteBatch->DrawTexture(Rect(xPos, yPos, 64, 64), m_pTexture1);
	}*/
	


	m_pSpriteBatch->DrawTexture(Rect(0, 200, 100, 100), m_pTexture2, Rect(0,0,0.12f, 0.18f));
	m_pSpriteBatch->DrawTexture(Rect(100, 200, 100, 100), m_pTexture2, Rect(0, 0, 0.12f, 0.18f));
	m_pSpriteBatch->DrawTexture(Rect(200, 200, 100, 100), m_pTexture2, Rect(0, 0.18, 0.09f, 0.18f));


	//m_pSpriteBatch->DrawTexture(Rect(600, 0, 200, 200), m_pTexture2, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));

	//m_pSpriteBatch->DrawTexture(Rect(850, 200, 200, 200), m_pTexture2, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));


	m_pSpriteBatch->End();
}