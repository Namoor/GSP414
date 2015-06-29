#pragma once

#include "d3dinclude.h"

struct TilingTerrain_Vertex
{
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Normal;
	D3DXVECTOR2 m_UV;
};

struct TilingTerrain_MatrixBuffer
{
	D3DXMATRIX m_WorldViewProjection;
};