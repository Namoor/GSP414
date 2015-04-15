#pragma once

#include <Windows.h>
#include "d3dinclude.h"

#include "Scene.h"

#include "Timer.h"

class System
{
public:
	System();
	~System();

	void Initialize();


	void Run(IScene* p_pScene);


private:
	void InitWindow();
	void InitD3D();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_Window;
	IScene* m_pScene;
	
	Timer* m_pTimer;

	// Kann man sich vorstellen wie der Monitor
	IDXGISwapChain* m_pSwapChain;

	// Kann man sich vorstellen, wie die Grafikkarte
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;


	ID3D11RenderTargetView* m_pBackbuffer;






};