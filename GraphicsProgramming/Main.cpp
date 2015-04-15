#include "System.h"

#include "2DScene.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System* m_pSystemInstance = new System();

	m_pSystemInstance->Initialize();


	m_pSystemInstance->Run(new Scene2D());

	//m_pSystemInstance->Run(new minecraftScene());


	return 0;
}