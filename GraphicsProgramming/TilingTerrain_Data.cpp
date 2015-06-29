#include "TilingTerrain_Data.h"



void TilingTerrain_Data::Init(int xSize, int ySize, TilingTerrainType p_Default)
{
	m_XSize = xSize;
	m_YSize = ySize;


	m_pData = new TilingTerrainType[m_XSize * m_YSize];

	for (int x = 0; x < m_XSize; x++)
	{
		for (int y = 0; y < m_YSize; y++)
		{
			SetData(x, y, p_Default);
		}
	}
}


void TilingTerrain_Data::SetData(int x, int y, TilingTerrainType p_Type)
{
	m_pData[x * m_XSize + y] = p_Type;
}

TilingTerrainType TilingTerrain_Data::GetData(int x, int y)
{
	return m_pData[x * m_XSize + y];
}