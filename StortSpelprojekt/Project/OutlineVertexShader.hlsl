struct VS_INPUT
{
    float3 position : POSITION;
    float2 textureCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
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

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.worldPosition = mul(float4(input.position, 1.0f), world);

    output.position = mul(output.worldPosition, viewPerspective);
    
    output.lightClipPosition = mul(output.worldPosition, lightViewPerspective);

    return output;
}