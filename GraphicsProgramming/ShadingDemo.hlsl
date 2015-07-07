struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
};

struct VertexIn
{
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
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

Texture2D gTexture;
SamplerState gSampler;


VertexOut VShader(VertexIn VInput)
{
	VertexOut Output;
	Output.pos = mul(WorldViewProjectionMatrix, float4(VInput.Position.xyz, 1));
	Output.col = VInput.Color;
	Output.UV = VInput.UV;
	Output.Normal = VInput.Normal;

	return Output;
}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	p_Input.Normal = normalize(p_Input.Normal);



	// Texture Mapping
	float4 TexColor = gTexture.Sample(gSampler, p_Input.UV + gTextureOffset);


		float DiffLightIntensity = saturate(dot(p_Input.Normal, float3(0, 1, 0)));

	float AmbientLightIntensity = 0.3;

	// Lighting
	float LightIntensity = saturate(AmbientLightIntensity + DiffLightIntensity);


	//                 	
	// float cos(a) = 	 dot(p_Input.Normal, float3(0,1,0))

	//                   



	return float4(TexColor.rgb * LightIntensity, 1);

	//return float4(LightIntensity, LightIntensity, LightIntensity, 1);
}