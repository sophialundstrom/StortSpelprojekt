
cbuffer CAMERA : register(b2)
{
    float3 cameraPosition;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 textureCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
};

cbuffer Color : register(b0)
{
    float3 color;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(color, 1.0f);
}