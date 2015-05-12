#include "Input.h"
#include <Windows.h>


Input* Input::s_pInput = nullptr;

Input* Input::GetInstance()
{
	if (s_pInput == nullptr)
	{
		s_pInput = new Input();
	}

	return s_pInput;
}

Input::Input()
{
	for (int x = 0; x < 256; x++)
	{
		m_KeyIsDown[x] = false;
		m_KeyWasDown[x] = false;
	}
}

void Input::Update()
{
	for (int x = 0; x < 256; x++)
	{
		m_KeyWasDown[x] = m_KeyIsDown[x];
		m_KeyIsDown[x] = GetAsyncKeyState(x);
	}
}

bool Input::GetKey(KeyCode::Key p_KeyCode)
{
	return m_KeyIsDown[p_KeyCode];
}

bool Input::GetKeyLastFrame(KeyCode::Key p_KeyCode)
{
	return m_KeyWasDown[p_KeyCode];
}

bool Input::GetKeyDown(KeyCode::Key p_KeyCode)
{
	return m_KeyIsDown[p_KeyCode] && !m_KeyWasDown[p_KeyCode];
}