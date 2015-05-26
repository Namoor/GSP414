struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
};

cbuffer Matrizen
{
	float4x4 WorldViewProjectionMatrix;
};

Texture2D gTexture;
SamplerState gSampler;


VertexOut VShader(float4 Position : POSITION0, float4 Color : COLOR0, float2 UV : TEXCOORD0, float3 Normal : NORMAL0)
{
	VertexOut Output;

	Output.pos = mul(WorldViewProjectionMatrix, Position);
	Output.col = Color;
	Output.UV = UV;
	Output.Normal = Normal;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	float LightIntensity = saturate( 0.3 + 0.7 * dot(p_Input.Normal , float3(0, -1, 0)));
	return 1.5 * p_Input.col * gTexture.Sample(gSampler, p_Input.UV) * LightIntensity;
}