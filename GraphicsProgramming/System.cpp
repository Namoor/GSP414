#include "System.h"

System::System()
{
	m_Window = 0;

	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
	m_pSwapChain = nullptr;

	m_pBackbuffer = nullptr;
}

System::~System()
{

}

void System::Initialize()
{
	InitWindow();
	InitD3D();

	m_pTimer = new Timer();
}

void System::InitWindow()
{
	// Schritt 1:
	// WindowsClass Struct erstellen

	WNDCLASSEX _wc;

	ZeroMemory(&_wc, sizeof(_wc));

	_wc.cbSize = sizeof(_wc);
	_wc.lpfnWndProc = WindowProc;
	_wc.style = CS_HREDRAW | CS_VREDRAW;
	_wc.hInstance = GetModuleHandle(NULL);
	_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wc.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
	_wc.lpszClassName = TEXT("WindowClassBauplan");

	// Schritt 2:
	// Window Class registrieren

	RegisterClassEx(&_wc);

	// Schritt 3:
	// WindowRect anlegen und anpassen

	DWORD WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	RECT _WndRect;

	_WndRect.top = 40;
	_WndRect.left = 400;
	_WndRect.bottom = 400;
	_WndRect.right = 800;
	AdjustWindowRect(&_WndRect, WindowStyle, false);


	// Schritt 4: Fenster erzeugen

	m_Window = CreateWindow(TEXT("WindowClassBauplan"),
		TEXT("Mein Erstes Fenster"),
		WindowStyle,
		_WndRect.left,
		_WndRect.top,
		_WndRect.right - _WndRect.left,
		_WndRect.bottom - _WndRect.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	// Schritt 5: Fenster anzeigen
		ShowWindow(m_Window, 10);


}

void System::InitD3D()
{
	HRESULT _Result;

	DXGI_SWAP_CHAIN_DESC _SwapChainDesc;
	ZeroMemory(&_SwapChainDesc, sizeof(_SwapChainDesc));

	_SwapChainDesc.BufferCount = 1;
	_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_SwapChainDesc.OutputWindow = m_Window;
	_SwapChainDesc.Windowed = true;
	_SwapChainDesc.SampleDesc.Count = 4;


	D3D_FEATURE_LEVEL _FeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL _FeatureLevelOut;

	//Device + DeviceContext symbolisieren die Grafikkarte
	//Swapchain symbolisiert Verbindung zwischen Frontbuffer und Backbuffer

	_Result = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0, //D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG,
		&_FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&_SwapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		&_FeatureLevelOut,
		&m_pDeviceContext);

	if (_Result != S_OK)
	{
		// Fehlermeldung ausgeben
		MessageBox(NULL, TEXT("DirectX Device konnte nicht erstellt werden Fehlercode: 0001"), TEXT("ERROR!"), MB_OKCANCEL);
		PostQuitMessage(0);
		return;
	}

	//ID3D11RenderTargetView für das Zeichnen/Rendern in eine Textur
	//ID3D11ShaderResourceView für das Zeichnen/Rendern von einer Textur

	ID3D11Texture2D* _pBackBufferTexture = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&_pBackBufferTexture);

	m_pDevice->CreateRenderTargetView(_pBackBufferTexture, nullptr, &m_pBackbuffer);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackbuffer, nullptr);


	D3D11_VIEWPORT _ViewPort;
	_ViewPort.TopLeftX = 0;
	_ViewPort.TopLeftY = 0;
	_ViewPort.Width = 1024;
	_ViewPort.Height = 1024;
	_ViewPort.MinDepth = 0;
	_ViewPort.MaxDepth = 1;

	m_pDeviceContext->RSSetViewports(1, &_ViewPort);

}



void System::Run(IScene* p_pScene)
{
	m_pScene = p_pScene;

	m_pScene->Initialize();
	float InitTime = m_pTimer->GetDeltaTime();

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		float TimeSinceLastFrame = m_pTimer->GetDeltaTime();

		float Color[4];
		Color[0] = (rand() % 256) / 256.0f; // Rot
		Color[1] = (rand() % 256) / 256.0f; // Grün
		Color[2] = (rand() % 256) / 256.0f; // Blau
		Color[3] = 1; // Alpha

		m_pDeviceContext->ClearRenderTargetView(m_pBackbuffer, Color);

		// Rendern der Scene
		m_pScene->Update(TimeSinceLastFrame);
		m_pScene->Render();

		m_pSwapChain->Present(0, 0);
	}
}

// ------------------- WindowProc --------------------------

LRESULT CALLBACK System::WindowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	default:
		break;
	}

	return DefWindowProc(hwnd, Message, wParam, lParam);
}