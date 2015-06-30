#include "2DScene.h"
#include "Texture.h"

#include "Input.h"

Scene2D::Scene2D()
{
	m_pFirstObject = nullptr;
	m_pSpriteBatch = nullptr;

	m_pPyramid = nullptr;
}

Scene2D::~Scene2D()
{
	if (m_pPyramid)
	{
		delete m_pPyramid;
		m_pPyramid = nullptr;
	}
}

void Scene2D::Initialize(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;

	m_pFirstObject = new Cube();
	m_pFirstObject->Initialize(p_pDevice, p_pDevCon);

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(p_pDevice, p_pDevCon);

	m_pTexture1 = new Texture("Coin.png", m_pDevice);
	m_pTexture2 = new Texture("SpriteSheet.png", m_pDevice);

	m_pArial = new SpriteFont("Font.fnt", m_pDevice);

	TilingTerrain_Data* _pData = new TilingTerrain_Data();
	_pData->Init(128, 128, TilingTerrainType::Grass);

	//_pData->SetData(2, 2, TilingTerrainType::Dirt);
	_pData->SetData(2, 1, TilingTerrainType::Dirt);
	_pData->SetData(1, 2, TilingTerrainType::Dirt);
	_pData->SetData(3, 2, TilingTerrainType::Dirt);
	_pData->SetData(2, 3, TilingTerrainType::Dirt);

	m_pPlane = new TilingTerrain();
	m_pPlane->Init(p_pDevice, p_pDevCon, 100, 100, _pData);

	m_pPyramid = new Pyramid();
	m_pPyramid->Init(p_pDevice, p_pDevCon);

	m_pFPSCounter = new FPSCounter();

	m_pCamera = new Camera();
}

void Scene2D::Update(float p_DeltaTime)
{
	m_pCamera->Update(p_DeltaTime);

	m_pPlane->Update(p_DeltaTime, m_pCamera);

	//m_pPyramid->Update(p_DeltaTime, m_pCamera);

	m_pFPSCounter->Update(p_DeltaTime);
	m_pFirstObject->Update(p_DeltaTime, m_pCamera);
}

void Scene2D::Render()
{
	m_pPlane->Draw();
	//m_pPyramid->Draw();

	//m_pFirstObject->Draw();

	//m_pFirstObject->Draw();
	m_pSpriteBatch->Begin();
	
	//if(Keydown Space) Zeichne Coins
	//if(Input::GetInstance()->GetKey(KeyCode::A))
	//for (int x = 0; x < 1000; x++)
	//{
	//	int xPos = rand() % 1000;
	//	int yPos = rand() % 1000;
	//	m_pSpriteBatch->DrawTexture(Rect(xPos, yPos, 64, 64), m_pTexture1);
	//}

	
	m_pSpriteBatch->DrawString(m_pArial, "Hello World", 200, 0, 48, D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawString(m_pArial, "the quick brown fox jumps over the lazy dog", 0, 48, 48, D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawString(m_pArial, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", 0, 2 * 48, 32, D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawString(m_pArial, "_ - ? = ) ( / & % $ . : , ; * ^ ~", 0, 3 * 48, 48, D3DXVECTOR4(1, 1, 1, 1));



	//m_pSpriteBatch->DrawTexture(Rect(0, 200, 100, 100), m_pTexture2, Rect(0,0,0.12f, 0.18f));
	//m_pSpriteBatch->DrawTexture(Rect(100, 200, 100, 100), m_pTexture2, Rect(0, 0, 0.12f, 0.18f));
	//m_pSpriteBatch->DrawTexture(Rect(200, 200, 100, 100), m_pTexture2, Rect(0, 0.18, 0.09f, 0.18f));


	//m_pSpriteBatch->DrawTexture(Rect(600, 0, 200, 200), m_pTexture2, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));

	//m_pSpriteBatch->DrawTexture(Rect(850, 200, 200, 200), m_pTexture2, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));




	m_pFPSCounter->Render(m_pSpriteBatch, m_pArial);
	m_pSpriteBatch->End();
}