#pragma once

#include "d3dinclude.h"

struct Plane_Vertex
{
	D3DXVECTOR3 m_Position;
	D3DXVECTOR4 m_Color;
	D3DXVECTOR2 m_UV;
	D3DXVECTOR3 m_Normal;
};

struct Plane_MatrixBuffer
{
	D3DXMATRIX m_WorldViewProjection;
};