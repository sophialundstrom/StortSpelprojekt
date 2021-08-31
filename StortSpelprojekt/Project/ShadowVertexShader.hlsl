struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

cbuffer HAS_DISPLACEMENT : register(b0)
{
    bool hasDisplacement;
};

cbuffer MATRIX : register(b1)
{
    float4x4 world;
    float4x4 WVP;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    if (hasDisplacement)
        output.position = float4(input.position, 1.0f);
    else
        output.position = mul(float4(input.position, 1.0f), WVP);
    
    output.texCoords = input.texCoords;
    output.normal = input.normal;
    
    return output;
}