struct VertexOut
{
	float4 pos : SV_POSITION;
	float3 Normal : NORMAL0;
	float2 UV : TEXCOORD0;
	float2 UV1 : TEXCOORD1;
	float2 UV2 : TEXCOORD2;
	float2 UV3 : TEXCOORD3;
};

struct VertexIn
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 UV : TEXCOORD0;
	float2 UV1 : TEXCOORD1;
	float2 UV2 : TEXCOORD2;
	float2 UV3 : TEXCOORD3;
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
	Output.UV1 = VInput.UV1;
	Output.UV2 = VInput.UV2;
	Output.UV3 = VInput.UV3;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	// Texture Mapping


	float4 GrassTex = gTexture.Sample(gSampler, p_Input.UV);
	float4 SnowTex = gTexture.Sample(gSampler, p_Input.UV1);
	float4 StoneTex = gTexture.Sample(gSampler, p_Input.UV2);
	float4 CityTex = gTexture.Sample(gSampler, p_Input.UV3);

	float CurrentAlpha = 1;

	float3 FinalColor = float3(0, 0, 0);

		FinalColor += GrassTex.rgb * GrassTex.a * CurrentAlpha;
	CurrentAlpha *= (1 - GrassTex.a);

	FinalColor += SnowTex.rgb * SnowTex.a * CurrentAlpha;
	CurrentAlpha *= (1 - SnowTex.a);

	FinalColor += StoneTex.rgb * StoneTex.a * CurrentAlpha;
	CurrentAlpha *= (1 - StoneTex.a);

	FinalColor += CityTex.rgb * CityTex.a * CurrentAlpha;
	//CurrentAlpha *= (1 - CityTex.a);

	
	
	//GrassTex.rgb *= GrassTex.a;

		// Lighting
		//float LightIntensity = 0.3 + 0.7 * saturate(dot(p_Input.Normal, float3(0, -1, 0)));

	return float4(FinalColor, 1);

	//return float4(LightIntensity, LightIntensity, LightIntensity, 1);
}