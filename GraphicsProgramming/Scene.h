#pragma once
#include "d3dinclude.h"

class IScene
{
public:
	// Initialisiert die Scene
	virtual void Initialize(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) = 0;


	// Updated die Scene
	virtual void Update(float p_DeltaTime) = 0;

	// Rendert die Scene
	virtual void Render() = 0;
};