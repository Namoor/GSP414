#pragma once

#include "d3dinclude.h"

struct Pyramid_Vertex
{
	D3DXVECTOR3 Position; // : POSITION0

	D3DXVECTOR3 Normal; // : NORMAL0
	D3DXVECTOR3 Color; // : COLOR0
	D3DXVECTOR2 UV; // : TEXCOORD0
};


struct Pyramid_MatrixBuffer
{
	D3DXMATRIX WorldViewProjectionMatrix;
};

