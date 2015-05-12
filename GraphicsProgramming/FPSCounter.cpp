#include "FPSCounter.h"


FPSCounter::FPSCounter()
{
	TimePassedSinceLastRefresh = 0;
	FramesSinceLastRefresh = 0;

	CurrentFPS = 0;
}

void FPSCounter::Update(float p_TimePassed)
{
	TimePassedSinceLastRefresh += p_TimePassed;
	FramesSinceLastRefresh += 1;


	// Refresh
	if (TimePassedSinceLastRefresh > 1.0f)
	{
		CurrentFPS = FramesSinceLastRefresh / TimePassedSinceLastRefresh;
		TimePassedSinceLastRefresh = 0;
		FramesSinceLastRefresh = 0;
	}
}

void FPSCounter::Render(SpriteBatch* p_pSpriteBatch, SpriteFont* p_pFont)
{
	char Buffer[200];

	sprintf_s(Buffer, "FPS: %d", CurrentFPS);

	p_pSpriteBatch->DrawString(p_pFont, Buffer, 0, 0, 48, D3DXVECTOR4(1, 1, 1, 1));
}