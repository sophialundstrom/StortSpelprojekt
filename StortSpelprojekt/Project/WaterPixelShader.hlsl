Texture2D diffuseTexture : register(t0);

sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
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

float4 main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;

    const float2 newTex = input.texCoords * 30.0f;

    float4 color = diffuseTexture.Sample(wrapSampler, newTex) * 0.8;

    //output.normal = float4(input.normal, 1.0f);

    //output.worldPosition = float4(input.worldPosition, 1.0f);

    //output.diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //output.specular = float4(1.0f, 1.0f, 1.0f, 1.0f);

    //output.ambient = float4(0.5f, 0.5f, 0.5f, 1.0f);

    //output.lightClipPosition = mul(output.worldPosition, lightMatrix);

    return color;
}