#include "LightCalculations.hlsli"
Texture2D blendTexture : register(t0);

Texture2D pathTexture : register (t1);

Texture2D textures[4] : register(t2);

sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 worldPosition : WORLDPOSITION;
};

cbuffer CAMERA : register(b2)
{
    float3 cameraPosition;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    const float4 blendValue = blendTexture.Sample(wrapSampler, input.texCoords);
    
    const float2 newTex = input.texCoords * 60.0f;
    
    const float4 t1 = textures[0].Sample(wrapSampler, newTex) * blendValue.x;
    const float4 t2 = textures[1].Sample(wrapSampler, newTex) * blendValue.y;
    const float4 t3 = textures[2].Sample(wrapSampler, newTex) * blendValue.z;
    const float4 t = t1 + t2 + t3;

    const float4 path = textures[3].Sample(wrapSampler, newTex);

    //FOG
    float4 fogColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
    float fogStart = 100.0f;
    float fogRange = 2000.0f;
    float fogDistance = distance(cameraPosition, input.worldPosition);
    float fogFactor = saturate((fogDistance - fogStart) / fogRange);
    
    float4 color = lerp(t, path, pathTexture.Sample(wrapSampler, input.texCoords).x);
    
    return lerp(color, fogColor, fogFactor);
}