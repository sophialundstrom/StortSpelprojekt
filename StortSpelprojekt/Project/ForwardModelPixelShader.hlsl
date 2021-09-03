Texture2D diffuseTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLDPOSITION;
    float2 texCoords : TEXCOORDS;
    float3 normal : NORMAL;
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
    return diffuseTexture.Sample(wrapSampler, input.texCoords);
}