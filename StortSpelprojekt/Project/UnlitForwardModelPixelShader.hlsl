Texture2D diffuseTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return diffuseTexture.Sample(wrapSampler, input.texCoords);
}