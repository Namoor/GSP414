struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 UV : TEXCOORD0;
	float4 WorldPos : TEXCOORD1;
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
	float4 DirectionalLightColor;
	float4 AmbientLightColor;
	float4 DirectionalLightDir;
	float4 CameraPosition;
};

Texture2D gTexture;
SamplerState gSampler;


VertexOut VShader(VertexIn VInput)
{
	VertexOut Output;
	Output.pos = mul(WorldViewProjectionMatrix, float4(VInput.Position.xyz, 1));
	Output.WorldPos = float4(VInput.Position.xyz, 0);
	Output.col = VInput.Color;
	Output.UV = VInput.UV;
	Output.Normal = VInput.Normal;
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
	float4 TexColor = gTexture.Sample(gSampler, p_Input.UV);

		// SpecLight
		float3 PointToLight = -DirectionalLightDir.xyz;
		float3 PointToCamera = normalize(CameraPosition.xyz - p_Input.WorldPos.xyz);

		float SpecularPower = BlinnPhongShading(PointToLight, PointToCamera, p_Input.Normal);

	SpecularPower = pow(SpecularPower, 10); // äquivalent zu: SpecularPower ^ 100

	float3 SpecularColor = DirectionalLightColor * saturate(SpecularPower);

	// DiffuseLight
	float3 DiffLightColor = saturate(dot(p_Input.Normal, -DirectionalLightDir.xyz)) * DirectionalLightColor;

	
	// AmbientLight
	float3 AmbientLightColor1 = AmbientLightColor;

	
	// Combination
	float3 LightColor = saturate(AmbientLightColor1 + DiffLightColor + SpecularColor);


	//                 	
	// float cos(a) = 	 dot(p_Input.Normal, float3(0,1,0))

	//                   



	return float4(LightColor * TexColor, 1);
	//return float4(AmbientLightColor, 1);

	//return float4(LightIntensity, LightIntensity, LightIntensity, 1);
}