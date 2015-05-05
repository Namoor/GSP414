#pragma once


#include "Scene.h"
#include "Texture.h"
#include "Simple2DObject.h"
#include "SpriteBatch.h"

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
	Simple2DObject* m_pFirstObject;
	Texture* m_pTexture1;

	SpriteBatch* m_pSpriteBatch;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;


};