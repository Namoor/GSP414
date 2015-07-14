struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 UV : TEXCOORD0;
	float4 WorldPos : TEXCOORD1;
	float3 Normal : NORMAL0;
	float3 Tangent : NORMAL1;
	float3 BiTangent : NORMAL2;
};

struct VertexIn
{
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float3 Tangent : NORMAL1;
	float3 BiTangent : NORMAL2;
};

cbuffer Matrizen // ConstantBuffer 0
{
	float4x4 WorldViewProjectionMatrix;
	float4x4 WorldMatrix;
};


cbuffer CPixel // Constantbuffer 1
{
	float4 DirectionalLightColor;
	float4 AmbientLightColor;
	float4 DirectionalLightDir;
	float4 CameraPosition;
	float4 RGBPointLightPosAPointLightRange;
};

Texture2D gTexture[2];	// 0: Diffuse
						// 1: Normal


SamplerState gSampler;


VertexOut VShader(VertexIn VInput)
{
	VertexOut Output;
	Output.pos = mul(WorldViewProjectionMatrix, float4(VInput.Position.xyz, 1));
	Output.WorldPos = mul(WorldMatrix, float4(VInput.Position.xyz, 1));
	Output.col = VInput.Color;
	Output.UV = VInput.UV;
	Output.Normal = mul(WorldMatrix, float4(VInput.Normal, 0));
	Output.Tangent = mul(WorldMatrix, float4(VInput.Tangent, 0));
	Output.BiTangent = mul(WorldMatrix, float4(VInput.BiTangent, 0));
	return Output;
}

float PhongShading(float3 PointToLight, float3 PointToCamera, float3 Normal)
{
	// Berechnung des Reflection Vectors
	float3 Reflected = normalize(-PointToLight + 2 * Normal * dot(Normal, PointToLight));
		// Äquivalent zu: float3 Reflected = -reflect(PointToLight, Normal);

	// SpecularPower returnen
	return saturate(dot(PointToCamera, Reflected));
}

float BlinnPhongShading(float3 PointToLight, float PointToCamera, float3 Normal)
{
	float3 HalfVector = normalize(PointToLight + PointToCamera);

		return saturate(dot(HalfVector, Normal));


}

float4 PShader(VertexOut p_Input) : SV_TARGET
{
	p_Input.Normal = normalize(p_Input.Normal);



	// Texture Mapping
	float4 TexColor = gTexture[0].Sample(gSampler, p_Input.UV);
		float3 Normal = gTexture[1].Sample(gSampler, p_Input.UV).rgb;

		Normal = Normal * 2 - 1;

	float3 TransformedNormal =	p_Input.Normal * Normal.b
								+ p_Input.Tangent * Normal.r
								+ p_Input.BiTangent * Normal.g;

	TransformedNormal = normalize(TransformedNormal);

		// SpecLight
		float3 PointToLight = -DirectionalLightDir.xyz;
		float3 PointToCamera = normalize(CameraPosition.xyz - p_Input.WorldPos.xyz);

		float SpecularPower = PhongShading(PointToLight, PointToCamera, TransformedNormal);

	SpecularPower = pow(SpecularPower, 100); // äquivalent zu: SpecularPower ^ 100

	float3 SpecularColor = DirectionalLightColor * saturate(SpecularPower);

	// DiffuseLight
	float3 DiffLightColor = saturate(dot(TransformedNormal, -DirectionalLightDir.xyz)) * DirectionalLightColor;

	
	// AmbientLight
	float3 AmbientLightColor1 = AmbientLightColor;

	
	// Combination
	float3 LightColor = saturate(AmbientLightColor1 + DiffLightColor + SpecularColor);


	//                 	
	// float cos(a) = 	 dot(p_Input.Normal, float3(0,1,0))

	//                   



	return float4(LightColor * TexColor, 1);
	//return float4(TransformedNormal, 1);

	//return float4(LightIntensity, LightIntensity, LightIntensity, 1);
}