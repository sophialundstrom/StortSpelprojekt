struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float3 position : POSITION;
    float2 textureCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
};

cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
    float4x4 lightViewPerspective;
}

float4 main(VS_INPUT input) : SV_POSITION
{
    float4x4 tempMatrix = mul(world, viewPerspective);
    return mul(float4(input.position, 1.0f), tempMatrix);
}