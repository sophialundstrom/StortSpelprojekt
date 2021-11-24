#include "LightCalculations.hlsli"
Texture2D diffuseTexture : register(t0);
Texture2D normalMapTex1 : register(t11);
Texture2D normalMapTex2 : register(t12);
Texture2D noiseTexture : register(t13);
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

cbuffer textureTranslation : register (b10)
{
    float offset;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    float2 nm1 = input.texCoords;
    nm1.x += offset;
    nm1.y -= offset * 0.5f;
    float2 nm2 = input.texCoords;
    nm2.x -= offset;
    nm2.y -= offset * 2.0f;
    float4 color = diffuseTexture.Sample(wrapSampler, input.texCoords * 50.0f);
    float4 normalMap1 = normalMapTex1.Sample(wrapSampler, nm1 * 100.0f);
    float4 normalMap2 = normalMapTex2.Sample(wrapSampler, nm2 * 50.0f);
    float4 noiseTex = noiseTexture.Sample(wrapSampler, input.texCoords * 50.0f);

    //Range from [0, 1] to [-1, 1]
    normalMap1.x = (2.0f * normalMap1.x) - 1.0f;
    normalMap1.y = (2.0f * normalMap1.y) - 1.0f;
    normalMap1.z = normalMap1.z;
    normalMap2.x = (2.0f * normalMap2.x) - 1.0f;
    normalMap2.y = (2.0f * normalMap2.y) - 1.0f;
    normalMap2.z = normalMap2.z;

    input.tangent = normalize(input.tangent - dot(input.tangent, input.normal) * input.normal);
    float3 biTangent = cross(input.normal, input.tangent);
    float3x3 texSpace = float3x3(input.tangent, biTangent, input.normal);
    input.normal = normalize(mul(normalMap1 + normalMap2, texSpace));

    //FOG
    float4 fogColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
    float fogStart = 100.0f;
    float fogRange = 2000.0f;
    float fogDistance = distance(cameraPosition, input.worldPosition);
    float fogFactor = saturate((fogDistance - fogStart) / fogRange);

    float3 finalColor;
    finalColor = color * directionalLight.lightColor;
    finalColor += saturate(dot(directionalLight.lightDirection, input.normal) * directionalLight.lightColor * color);

    finalColor += ((input.worldPosition.y + 0.5f) * (noiseTex * 1.5f) * 0.2f);

    return float4((lerp((input.worldPosition, 1.0f) * saturate(finalColor), fogColor, fogFactor)), color.a);
}