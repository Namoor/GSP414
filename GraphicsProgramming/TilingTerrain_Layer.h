#pragma once



#include "TilingTerrain_Data.h"


class TilingTerrain_Layer
{
private:
	float m_TexX;
	float m_TexY;
	float m_TexXSize;
	float m_TexYSize;

	bool m_HasVariations;
	TilingTerrainType m_Type;

public:
	TilingTerrain_Layer(float p_TexX, float p_TexY, float p_TexXSize, float p_TexYSize, bool p_HasVariation, TilingTerrainType p_Type);


	void GetTexCoords(TilingTerrainType p_OL, TilingTerrainType p_OR, TilingTerrainType p_UL, TilingTerrainType p_UR, float* p_pTexX, float* p_pTexY, bool Fill);
};