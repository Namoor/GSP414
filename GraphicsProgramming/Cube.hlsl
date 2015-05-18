struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 UV : TEXCOORD;
};

cbuffer Matrizen
{
	float4x4 WorldMatrix;
};

Texture2D gTexture;
SamplerState gSampler;


VertexOut VShader(float4 Position : POSITION, float4 Color : COLOR, float2 UV : TEXCOORD)
{
	VertexOut Output;

	Output.pos = mul(WorldMatrix, Position);
	Output.col = Color;
	Output.UV = UV;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	return p_Input.col * gTexture.Sample(gSampler, p_Input.UV);
}