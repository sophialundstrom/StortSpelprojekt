#include "LightCalculations.hlsli"
Texture2D diffuseTexture : register(t0);
Texture2D normalMapTex : register(t11);
sampler wrapSampler : register(s0);

cbuffer DIRECTIONALLIGHT : register(b1)
{
    DirectionalLight directionalLight;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 worldPosition : WORLDPOSITION;
};

struct PS_OUTPUT
{
    float4 diffuseTexture : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 worldPosition : SV_TARGET2;
    float4 diffuse : SV_TARGET3;
    float4 ambient : SV_TARGET4;
    float4 specular : SV_TARGET5;
    float4 lightClipPosition : SV_TARGET6;
};

cbuffer test : register(b0)
{
    float4x4 lightMatrix;
}

cbuffer CAMERA : register(b2)
{
    float3 cameraPosition;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;

    const float2 newTex = input.texCoords * 30.0f;

    float4 color = diffuseTexture.Sample(wrapSampler, newTex) * 0.8;
    float3 normalMap = normalMapTex.Sample(wrapSampler, newTex).xyz;

    normalMap = (2.0f * normalMap) - 1.0f; //Range from [0, 1] to [-1, 1]
    input.tangent = normalize(input.tangent - dot(input.tangent, input.normal) * input.normal);
    float3 biTangent = cross(input.normal, input.tangent);
    float3x3 texSpace = float3x3(input.tangent, biTangent, input.normal);
    input.normal = normalize(mul(normalMap, texSpace));

    //FOG
    float4 fogColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
    float fogStart = 100.0f;
    float fogRange = 2000.0f;
    float fogDistance = distance(cameraPosition, input.worldPosition);
    float fogFactor = saturate((fogDistance - fogStart) / fogRange);

    float3 finalColor;
    float3 ambient = float3(0.8f, 0.8f, 0.8f);
    finalColor = color * directionalLight.lightColor;
    finalColor += saturate(dot(directionalLight.lightDirection, input.normal) * directionalLight.lightColor * color);

    return float4((lerp((input.worldPosition, 1.0f) * saturate(finalColor), fogColor, fogFactor)), color.a);
}