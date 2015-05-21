#pragma once


#include "Scene.h"
#include "Texture.h"
#include "Cube.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "FPSCounter.h"
#include "Plane.h"
#include "Camera.h"

class Scene2D : public IScene
{
public:
	Scene2D();
	~Scene2D();


public:
	virtual void Initialize(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;


	virtual void Update(float p_DeltaTime) override;
	
	virtual void Render() override;

private:
	Cube* m_pFirstObject;
	Texture* m_pTexture1;
	Texture* m_pTexture2;

	Plane* m_pPlane;

	SpriteBatch* m_pSpriteBatch;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	SpriteFont* m_pArial;


	FPSCounter* m_pFPSCounter;

	Camera* m_pCamera;
};