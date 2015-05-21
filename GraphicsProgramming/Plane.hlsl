struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

cbuffer Matrizen
{
	float4x4 WorldViewProjectionMatrix;
};

Texture2D gTexture;
SamplerState gSampler;


VertexOut VShader(float4 Position : POSITION0, float4 Color : COLOR0)
{
	VertexOut Output;

	Output.pos = mul(WorldViewProjectionMatrix, Position);
	Output.col = Color;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	return p_Input.col;
}