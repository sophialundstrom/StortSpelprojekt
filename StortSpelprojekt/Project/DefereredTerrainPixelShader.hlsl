// Array of textures since we are also blending based on the height value of the displacement
Texture2D diffuseTexture : register(t0);

sampler wrapSampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
    float4 worldPosition: WORLDPOSITION;
    float3 worldPos : WORLDPOS;
};

struct PSOutput
{
    float4 position : SV_TARGET0;
    float3 normal : SV_TARGET1;
    float3 worldPos : SV_TARGET2;
    float4 diffuse : SV_TARGET3;
    float4 ambientMTL : SV_TARGET4;
    float4 diffuseMTL : SV_TARGET5;
    float4 specularMTL : SV_TARGET6;
    float4 lightClipPos : SV_TARGET7;
};

cbuffer LightMatrix : register(b0)
{
    float4x4 lightMatrix;
}

cbuffer mtlData : register(b1)
{
    float4 kA;
    float4 kD;
    float4 kS;
}

PSOutput main(PSInput input)
{
    PSOutput output;

    output.worldPos = input.worldPos;
    output.normal = input.normal;
    output.position = input.position;
    output.ambientMTL = kA;
    output.diffuseMTL = kD;
    output.specularMTL = kS;

    // Blend with lerp
    output.diffuse = diffuseTexture.Sample(wrapsampler, input.texCoords)

    output.lightClipPos = mul(float4(output.worldPos, 1.0f), lightMatrix);

    return output;
}