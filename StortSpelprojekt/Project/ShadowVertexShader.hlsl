struct VS_INPUT
{
    float3 position : POSITION;
    float2 textureCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

cbuffer MATRIX : register(b0)
{
    float4x4 WVP;
};

float4 main(VS_INPUT input) : SV_POSITION
{
    return mul(float4(input.position, 1.0f), WVP);
}