#pragma once

#include "SpriteBatch.h"

class FPSCounter
{
public:
	FPSCounter();

	void Update(float p_DeltaTime);
	void Render(SpriteBatch* p_pSpriteBatch, SpriteFont* p_pFont);
private:
	float TimePassedSinceLastRefresh;
	int FramesSinceLastRefresh;

	int CurrentFPS;
};