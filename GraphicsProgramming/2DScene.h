#pragma once


#include "Scene.h"

class Scene2D : public IScene
{
public:
	Scene2D();
	~Scene2D();


public:
	virtual void Initialize() override;


	virtual void Update(float p_DeltaTime) override;
	
	virtual void Render() override;
};