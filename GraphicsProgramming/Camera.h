#pragma once

#include "d3dinclude.h"
#include "Input.h"

class Camera
{
public:
	Camera();

	D3DXMATRIX GetProjectionMatrix();
	D3DXMATRIX GetViewMatrix();

	void Update(float DeltaTime);

	void Move(D3DXVECTOR3 p_Movement, bool IsInWorldSpace);

	void UpdateViewMatrix();

private:
	D3DXMATRIX m_Projection;
	D3DXMATRIX m_View;

	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 m_Direction;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_Right;
};