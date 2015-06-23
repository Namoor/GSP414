struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 UV_Splat : TEXCOORD0;
	float2 UV : TEXCOORD1;
	float3 Normal : NORMAL0;
};

struct VertexIn
{
	float4 Color : COLOR0;
	float2 UV_Splat : TEXCOORD0;
	float2 UV : TEXCOORD1;
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
};

cbuffer Matrizen // ConstantBuffer 0
{
	float4x4 WorldViewProjectionMatrix;
};


cbuffer CPixel // Constantbuffer 1
{
	float2 gTextureOffset;
};

Texture2D gTexture[5];
//Texture2D gGrassTexture;

SamplerState gSampler;


VertexOut VShader(VertexIn VInput)
{
	VertexOut Output;

	Output.pos = mul(WorldViewProjectionMatrix, float4(VInput.Position.xyz, 1));
	Output.col = VInput.Color;
	Output.UV = VInput.UV;
	Output.UV_Splat = VInput.UV_Splat;
	Output.Normal = VInput.Normal;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	// Texture Mapping
	float4 SplatMap = gTexture[0].Sample(gSampler, p_Input.UV_Splat);

	float4 StoneTex = gTexture[1].Sample(gSampler, p_Input.UV); // *50);
	float4 GrassTex = gTexture[2].Sample(gSampler, p_Input.UV); // * 50);
	float4 SandTex = gTexture[3].Sample(gSampler, p_Input.UV); // * 50);
	float4 SnowTex = gTexture[4].Sample(gSampler, p_Input.UV); // * 50);


	float StoneStrength = SplatMap.r;
	float GrassStrength = SplatMap.g;
	float SandStrength = SplatMap.b;

	// optional
	float TotalAmmount = StoneStrength + GrassStrength + SandStrength;
	if (TotalAmmount > 1)
	{
		StoneStrength /= TotalAmmount;
		GrassStrength /= TotalAmmount;
		SandStrength /= TotalAmmount;
	}

	float SnowStrength = 1 - StoneStrength - GrassStrength - SandStrength;


	float3 TexColor = StoneTex * StoneStrength + GrassTex * GrassStrength + SandTex * SandStrength + SnowTex * SnowStrength;


	// Lighting
	float LightIntensity = 0.3 + 0.7 * saturate(dot(p_Input.Normal, float3(0, -1, 0)));
	return float4(1.5 * p_Input.col.rgb * TexColor.rgb * LightIntensity, 1);

	//return float4(LightIntensity, LightIntensity, LightIntensity, 1);
}