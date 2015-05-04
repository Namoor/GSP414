#include "2DScene.h"



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
	m_pFirstObject = new Simple2DObject();
	m_pFirstObject->Initialize(p_pDevice, p_pDevCon);

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(p_pDevice, p_pDevCon);
}

void Scene2D::Update(float p_DeltaTime)
{

}

void Scene2D::Render()
{
	m_pFirstObject->Draw();
}