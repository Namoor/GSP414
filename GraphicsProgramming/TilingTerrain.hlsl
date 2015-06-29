struct VertexOut
{
	float4 pos : SV_POSITION;
	float3 Normal : NORMAL0;
	float2 UV : TEXCOORD0;
};

struct VertexIn
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 UV : TEXCOORD0;
};

cbuffer Matrizen // ConstantBuffer 0
{
	float4x4 WorldViewProjectionMatrix;
};

Texture2D gTexture;
SamplerState gSampler;


VertexOut VShader(VertexIn VInput)
{
	VertexOut Output;

	Output.pos = mul(WorldViewProjectionMatrix, float4(VInput.Position.xyz, 1));
 	Output.Normal = VInput.Normal;
	Output.UV = VInput.UV;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	// Texture Mapping

	float4 GrassTex = gTexture.Sample(gSampler, p_Input.UV); // * 50);

		// Lighting
		float LightIntensity = 0.3 + 0.7 * saturate(dot(p_Input.Normal, float3(0, -1, 0)));

	return float4(1.0 * GrassTex.rgb * 1, 1);

	//return float4(LightIntensity, LightIntensity, LightIntensity, 1);
}