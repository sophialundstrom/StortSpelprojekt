Texture2D mask : register(t0);

SamplerState wrapSampler : register(s0);

cbuffer COLOR : register(b0)
{
    float3 color;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(color, mask.Sample(wrapSampler, input.texCoords).r);
}