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
	Output.col = float4(Color.rgb, 1);
	Output.UV = UV;
	Output.Normal = Normal;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	float LightIntensity = 0.3 + saturate(0.7 * dot(p_Input.Normal, float3(0, 1, 0)));
	float4 TexColor = gTexture.Sample(gSampler, p_Input.UV);
	return float4(1.5 * p_Input.col.rgb * TexColor.rgb * LightIntensity, 1);
}