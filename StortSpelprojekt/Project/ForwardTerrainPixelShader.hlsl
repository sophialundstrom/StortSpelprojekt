Texture2D blendTexture : register(t0);

Texture2D textures[3] : register(t1);

sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 worldPosition : WORLDPOSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    const float4 blendValue = blendTexture.Sample(wrapSampler, input.texCoords);
    
    const float2 newTex = input.texCoords * 40.0f;
    
    const float4 t1 = textures[0].Sample(wrapSampler, newTex) * blendValue.x;
    const float4 t2 = textures[1].Sample(wrapSampler, newTex) * blendValue.y;
    const float4 t3 = textures[2].Sample(wrapSampler, newTex) * blendValue.z;
    
    return t1 + t2 + t3;
}