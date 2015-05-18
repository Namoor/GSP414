#include "Camera.h"



Camera::Camera()
{
	m_Direction = D3DXVECTOR3(-3, -3, -3);
	m_Up = D3DXVECTOR3(0, 1, 0);
	m_Target = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Cross(&m_Right, &m_Up, &m_Direction);


	UpdateViewMatrix();
	
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DXToRadian(50), 1, 0.1f, 1000);

}

void Camera::UpdateViewMatrix()
{
	D3DXVECTOR3 _CameraPosition = m_Target - m_Direction;

	D3DXMatrixLookAtLH(&m_View, &_CameraPosition, &m_Target, &m_Up);

}

void Camera::Update(float DeltaTime)
{
	float xMovement = 0;

	if (Input::GetInstance()->GetKey(KeyCode::A))
		xMovement -= 1;

	if (Input::GetInstance()->GetKey(KeyCode::D))
		xMovement += 1;

	D3DXVECTOR3 Movement(xMovement * DeltaTime, 0, 0);

	Move(Movement, true);
}

void Camera::Move(D3DXVECTOR3 p_Movement, bool IsWorldSpace)
{
	if (IsWorldSpace)
	{
		m_Target += p_Movement;
	}
	else
	{
		// TODO
	}

	UpdateViewMatrix();
}

D3DXMATRIX Camera::GetProjectionMatrix()
{
	return m_Projection;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return m_View;
}