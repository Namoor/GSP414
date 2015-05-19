#include "Camera.h"



Camera::Camera()
{
	m_Direction = D3DXVECTOR3(-3, -3, -3);
	m_Up = D3DXVECTOR3(0, 1, 0);
	m_Target = D3DXVECTOR3(0, 0, 0);


	m_DistanceToTarget = 5;
	D3DXVec3Cross(&m_Right, &m_Up, &m_Direction);
	D3DXVec3Cross(&m_Up, &m_Direction, &m_Right);
	//Unity: m_Right = m_Right.normalized();
	D3DXVec3Normalize(&m_Right, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);
	D3DXVec3Normalize(&m_Direction, &m_Direction);


	UpdateViewMatrix();
	
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DXToRadian(50), 1, 0.1f, 1000);

}

void Camera::UpdateViewMatrix()
{
	D3DXVECTOR3 _CameraPosition = m_Target - m_Direction * m_DistanceToTarget;

	D3DXMatrixLookAtLH(&m_View, &_CameraPosition, &m_Target, &m_Up);

}

void Camera::Update(float DeltaTime)
{
	float xMovement = 0;
	float yMovement = 0;
	float zMovement = 0;

	float RotationX = 0;

	if (Input::GetInstance()->GetKey(KeyCode::A))
		xMovement -= 1;

	if (Input::GetInstance()->GetKey(KeyCode::D))
		xMovement += 1;

	if (Input::GetInstance()->GetKey(KeyCode::Shift))
		yMovement -= 1;

	if (Input::GetInstance()->GetKey(KeyCode::Space))
		yMovement += 1;

	if (Input::GetInstance()->GetKey(KeyCode::S))
		zMovement -= 1;

	if (Input::GetInstance()->GetKey(KeyCode::W))
		zMovement += 1;


	if (Input::GetInstance()->GetKeyDown(KeyCode::RightMouse))
	{
		m_SavedMousePos = Input::GetInstance()->GetMousePosition();
		ShowCursor(false);
		Input::GetInstance()->SetMousePosition(D3DXVECTOR2(400, 400));
	}

	if (Input::GetInstance()->GetKey(KeyCode::RightMouse))
	{
		D3DXVECTOR2 _Diff = Input::GetInstance()->GetMousePosition() - D3DXVECTOR2(400, 400);
		RotateX(_Diff.y * 0.02f, false);
		RotateY(_Diff.x * 0.02f, true);
		Input::GetInstance()->SetMousePosition(D3DXVECTOR2(400, 400));
	}

	if (Input::GetInstance()->GetKeyLastFrame(KeyCode::RightMouse) && !Input::GetInstance()->GetKey(KeyCode::RightMouse))
	{
		Input::GetInstance()->SetMousePosition(m_SavedMousePos);
		ShowCursor(true);
	}




	D3DXVECTOR3 Movement(xMovement * DeltaTime, yMovement * DeltaTime, zMovement * DeltaTime);

	Move(Movement, false);

	RotateY(RotationX * DeltaTime, false);
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
		m_Target += m_Right * p_Movement.x + m_Up * p_Movement.y + m_Direction * p_Movement.z;
	}

	UpdateViewMatrix();
}

void Camera::RotateX(float p_RotationAmmount, bool IsInWorldSpace)
{
	D3DXMATRIX _Rotation;

	if (IsInWorldSpace)
	{
		D3DXMatrixRotationX(&_Rotation, p_RotationAmmount);
	}
	else
	{
		D3DXMatrixRotationAxis(&_Rotation, &m_Right, p_RotationAmmount);
	}

	D3DXVECTOR4 _Output;

	// Transformation mithilfe eines Vector4, danach wieder umwandeln in Vector3
	D3DXVec3Transform(&_Output, &m_Direction, &_Rotation);
	m_Direction = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Up, &_Rotation);
	m_Up = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Right, &_Rotation);
	m_Right = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	UpdateViewMatrix();
}

void Camera::RotateY(float p_RotationAmmount, bool IsInWorldSpace)
{
	D3DXMATRIX _Rotation;

	if (IsInWorldSpace)
	{
		D3DXMatrixRotationY(&_Rotation, p_RotationAmmount);
	}
	else
	{
		D3DXMatrixRotationAxis(&_Rotation, &m_Up, p_RotationAmmount);
	}

	D3DXVECTOR4 _Output;

	// Transformation mithilfe eines Vector4, danach wieder umwandeln in Vector3
	D3DXVec3Transform(&_Output, &m_Direction, &_Rotation);
	m_Direction = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Up, &_Rotation);
	m_Up = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Right, &_Rotation);
	m_Right = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	UpdateViewMatrix();
}

void Camera::RotateZ(float p_RotationAmmount, bool IsInWorldSpace)
{
	D3DXMATRIX _Rotation;

	if (IsInWorldSpace)
	{
		D3DXMatrixRotationZ(&_Rotation, p_RotationAmmount);
	}
	else
	{
		D3DXMatrixRotationAxis(&_Rotation, &m_Direction, p_RotationAmmount);
	}

	D3DXVECTOR4 _Output;

	// Transformation mithilfe eines Vector4, danach wieder umwandeln in Vector3
	D3DXVec3Transform(&_Output, &m_Direction, &_Rotation);
	m_Direction = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Up, &_Rotation);
	m_Up = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Right, &_Rotation);
	m_Right = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

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