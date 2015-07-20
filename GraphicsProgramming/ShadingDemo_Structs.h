#pragma once

#include "d3dinclude.h"

struct ShadingDemo_Vertex
{
	D3DXVECTOR3 m_Position;
	D3DXVECTOR4 m_Color;
	D3DXVECTOR2 m_UV;
	D3DXVECTOR3 m_Normal;
	D3DXVECTOR3 m_Tangent;
	D3DXVECTOR3 m_BiTangent;
};

struct ShadingDemo_MatrixBuffer
{
	D3DXMATRIX m_WorldViewProjection;
	D3DXMATRIX m_WorldMatrix;
};

struct ShadingDemo_LightingBuffer
{
	D3DXVECTOR4 DirectionalLightColor;
	D3DXVECTOR4 AmbientLightColor;
	D3DXVECTOR4 DirectionalLightDir;
	D3DXVECTOR4 CameraPosition;
	D3DXVECTOR4 PointLightColor;
	D3DXVECTOR3 PointLightPos;
	float PointLightRange;
	D3DXVECTOR4 PointLightColor2;
	D3DXVECTOR3 PointLightPos2;
	float PointLightRange2;
};