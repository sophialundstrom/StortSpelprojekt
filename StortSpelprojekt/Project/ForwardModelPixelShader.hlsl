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
    float2 texCoords : TEXCOORDS;
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

float4 main(PS_INPUT input) : SV_TARGET
{
    return diffuseTexture.Sample(wrapSampler, input.texCoords) * LightCalculation(input.lightClipPosition, input.worldPosition, input.normal, diffuse, specular, ambient, directionalLight, cameraPosition, shadowMap, wrapSampler);
}