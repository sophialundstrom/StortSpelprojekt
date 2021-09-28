Texture2D diffuseTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 worldPosition : WORLDPOSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(input.texCoords, 0.0f, 1.0f); //diffuseTexture.Sample(wrapSampler, input.texCoords);
}