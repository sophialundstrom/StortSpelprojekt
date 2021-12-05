#include "LightCalculations.hlsli"
Texture2D diffuseTexture : register(t0);

Texture2D shadowMap : register(t10);
SamplerState wrapSampler : register(s0);

cbuffer DIRECTIONALLIGHT : register(b1)
{
    DirectionalLight directionalLight;
}

cbuffer CAMERA : register(b2)
{
    float3 cameraPosition;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
};

cbuffer MATERIAL : register(b0)
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float specularPower;
}

float ShadowCalculation(float4 LCP)
{
    LCP.xyz /= LCP.w; //PERSPECTIVE DIVIDE (NDC-COORDS)
    
    const float2 tx = float2(0.5f * LCP.x + 0.5f, -0.5f * LCP.y + 0.5f); // [-1,1] => [0, 1]
    
    const float sm = shadowMap.Sample(wrapSampler, tx).r;
    
    float shadow = (sm + 0.005 < LCP.z) ? 0.0f : 1.0f; //if closest depth (sample) < pixel-depth there is a primitive in front castings shadow.

    if (tx.x > 1.0f || tx.x < 0.0f ||
        tx.y > 1.0f || tx.y < 0.0f ||
        LCP.z > 1.0f || LCP.z < 0.0f)
        shadow = 1.0f;

    return shadow;
}

cbuffer NUM_POINTLIGHTS : register(b3)
{
	uint numPointlights;
}

StructuredBuffer<POINT_LIGHT> lights : register(t9);

float4 main(PS_INPUT input) : SV_TARGET
{
   	const float shadow = ShadowCalculation(input.lightClipPosition);
	const float4 globalAmbient = 0.15f;

	const float4 T = diffuseTexture.Sample(wrapSampler, input.texCoords);
    
	LightResult pResult = { { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
    

	for (uint i = 0; i < numPointlights; ++i)
	{
		const POINT_LIGHT pointLight = lights[i];
        LightResult result = PointLightCalculation(input.worldPosition, input.normal, diffuse, specular, pointLight.color, pointLight.intensity, pointLight.position, pointLight.range, pointLight.attenuation, cameraPosition);
        pResult.diffuse += result.diffuse;
		pResult.color *= result.color;
		pResult.specular += result.specular;
	}
    
    pResult.diffuse = saturate(pResult.diffuse);
    pResult.color = saturate(pResult.color);
    pResult.specular = saturate(pResult.specular);
    
    const float4 finalLighting = LightCalculation(input.worldPosition, input.normal, diffuse, specular, ambient, directionalLight, cameraPosition) * shadow +
                                (pResult.diffuse * 5 + pResult.specular) + globalAmbient + ambient;
    
    //FOG
	float4 fogColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
	float fogStart = 100.0f;
	float fogRange = 2000.0f;
	float fogDistance = distance(cameraPosition, input.worldPosition.xyz);
	float fogFactor = saturate((fogDistance - fogStart) / fogRange);

    //RESULT
    const float4 finalColor = lerp((T * float4(pResult.color) * (saturate(finalLighting))), fogColor, fogFactor);

    //return pResult.diffuse;
    return finalColor;
}