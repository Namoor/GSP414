#pragma once

#include "d3dinclude.h"

struct S2DO_Vertices
{
	D3DXVECTOR3 Position; // 3 floats = 12 Byte
	D3DXVECTOR4 Color; // 4 floats = 16 Byte
	D3DXVECTOR2 UV; // 2 floats = 8 Byte
};