struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    uint id : ID;
};

cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
}

cbuffer ID : register(b1)
{
    uint id;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = mul(mul(float4(input.position, 1.0f), world), viewPerspective);

    output.id = id;

    return output;
}