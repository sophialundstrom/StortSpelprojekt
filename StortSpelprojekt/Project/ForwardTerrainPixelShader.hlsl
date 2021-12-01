#include "LightCalculations.hlsli"
Texture2D blendTexture : register(t0);

Texture2D pathTexture : register (t1);

Texture2D textures[4] : register(t2);

Texture2D shadowMap : register(t10);

sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 worldPosition : WORLDPOSITION;
	float4 lightClipPosition : LIGHTPOSITION;
};

cbuffer DIRECTIONALLIGHT : register(b1)
{
    DirectionalLight directionalLight;
}

cbuffer CAMERA : register(b2)
{
    float3 cameraPosition;
}

cbuffer NUM_POINTLIGHTS : register(b3)
{
    uint numPointlights;
}

StructuredBuffer<POINT_LIGHT> lights : register(t9);

float ShadowCalculation(float4 LCP)
{
	LCP.xyz /= LCP.w; //PERSPECTIVE DIVIDE (NDC-COORDS)
    
	const float2 tx = float2(0.5f * LCP.x + 0.5f, -0.5f * LCP.y + 0.5f); // [-1,1] => [0, 1]
    const float sm = shadowMap.Sample(wrapSampler, tx).r;
    float shadow = (sm + 0.005 < LCP.z) ? 0.3f : 1.0f; //if closest depth (sample) < pixel-depth there is a primitive in front castings shadow.

	if (tx.x > 1.0f || tx.x < 0.0f ||
        tx.y > 1.0f || tx.y < 0.0f ||
        LCP.z > 1.0f || LCP.z < 0.0f)
		shadow = 1.0f;

	return shadow;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    const float4 blendValue = blendTexture.Sample(wrapSampler, input.texCoords);
    
    const float2 newTex = input.texCoords * 60.0f;
    
	const float shadow = ShadowCalculation(input.lightClipPosition);
    
    const float4 t1 = textures[0].Sample(wrapSampler, newTex) * blendValue.x;
    const float4 t2 = textures[1].Sample(wrapSampler, newTex) * blendValue.y;
    const float4 t3 = textures[2].Sample(wrapSampler, newTex) * blendValue.z;
    const float4 t = t1 + t2 + t3;

    const float4 path = textures[3].Sample(wrapSampler, newTex);
    
    float4 globalAmbient = (0.2f, 0.2f, 0.2f, 1);
    
    LightResult pResult = { { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };

    for (uint i = 0; i < numPointlights; ++i)
    {
        const POINT_LIGHT pointLight = lights[i];
        LightResult result = PointLightCalculation(float4(input.worldPosition.x, input.worldPosition.y, input.worldPosition.z, 1.f), input.normal, 0.f, 0.2f, pointLight.color, pointLight.intensity, pointLight.position, pointLight.range, pointLight.attenuation, cameraPosition);
        pResult.diffuse += result.diffuse;
        pResult.color *= result.color;
        pResult.specular += result.specular; 
    }
    
    pResult.diffuse = saturate(pResult.diffuse);
    pResult.color = saturate(pResult.color);
    pResult.specular = saturate(pResult.specular);
    
    LightResult finalLighting = DirectionalLightCalculation(float4(input.worldPosition, 1.f), input.normal, float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f), directionalLight.lightColor, directionalLight.lightDirection, cameraPosition);
    
    finalLighting.diffuse = saturate(finalLighting.diffuse);
    finalLighting.color = saturate(finalLighting.color);
    finalLighting.specular = saturate(finalLighting.specular);
    
    //FOG
    float4 fogColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
    float fogStart = 100.0f;
    float fogRange = 2000.0f;
    float fogDistance = distance(cameraPosition, input.worldPosition);
    float fogFactor = saturate((fogDistance - fogStart) / fogRange);

    
    float4 color = lerp(t, path, pathTexture.Sample(wrapSampler, input.texCoords).x);
    
    const float4 finalColor = lerp((color * finalLighting.color * saturate(finalLighting.color)), fogColor, fogFactor);
    
    return finalColor * shadow + globalAmbient;
   // return directionalLight.lightColor;
	//return lerp(color, fogColor, fogFactor) * shadow;

}