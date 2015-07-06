#include "TilingTerrain_Layer.h"
#include <random>

TilingTerrain_Layer::TilingTerrain_Layer(float p_TexX, float p_TexY, float p_TexXSize, float p_TexYSize, bool p_HasVariation, TilingTerrainType p_Type)
{
	m_TexX = p_TexX;
	m_TexY = p_TexY;
	m_TexXSize = p_TexXSize;
	m_TexYSize = p_TexYSize;

	m_HasVariations = p_HasVariation;
	m_Type = p_Type;
}

void TilingTerrain_Layer::GetTexCoords(TilingTerrainType p_OL, TilingTerrainType p_OR, TilingTerrainType p_UL, TilingTerrainType p_UR, float* p_pTexX, float* p_pTexY, bool Fill)
{



	int TexCoordX = 4;
	int TexCoordY = 4;

	int _TexCoordID = 0;       // 0000



	if (p_UR == m_Type) _TexCoordID |= 1; // 0001
	if (p_UL == m_Type) _TexCoordID |= 2; // 0010
	if (p_OR == m_Type) _TexCoordID |= 4; // 0100
	if (p_OL == m_Type) _TexCoordID |= 8; // 1000

	if (Fill)
	{
		_TexCoordID = 15;
	}


	TexCoordX = _TexCoordID % 4;
	TexCoordY = _TexCoordID / 4;


	if (_TexCoordID == 15 && m_HasVariations)
	{
		int _Rnd = rand() % 101;

		if (_Rnd  < 70)
		{
			// Eines der Random Tiles auswählen
			int _RandomTileID = rand() % 8;

			if (_Rnd < 15 && _Rnd > 5)
				_RandomTileID = rand() % 4 + 8;
			if (_Rnd < 5)
				_RandomTileID = rand() % 4 + 12;

			TexCoordX = 4 + _RandomTileID % 4;
			TexCoordY = _RandomTileID / 4;
		}
	}


	*p_pTexX = TexCoordX * 0.125f * m_TexXSize + m_TexX;
	*p_pTexY = TexCoordY * 0.25f * m_TexYSize + m_TexY;

}
