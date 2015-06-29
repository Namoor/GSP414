#pragma once


enum TilingTerrainType
{
	Grass,
	Stone,
	Dirt
};


class TilingTerrain_Data
{
public:
	void Init(int xSize, int ySize, TilingTerrainType p_DefaultTex);

	void SetData(int x, int y, TilingTerrainType p_Type);
	TilingTerrainType GetData(int x, int y);

private:
	TilingTerrainType* m_pData;
	int m_XSize;
	int m_YSize;
};