struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLDPOSITION;
    float2 texCoords : TEXCOORDS;
    float3 normal : NORMAL;
};

cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 position = float4(input.position, 1.0f);
    
    output.worldPosition = mul(position, world);
    output.position = mul(output.worldPosition, viewPerspective);
    
    output.texCoords = input.texCoords;
    
    output.normal = mul(float4(input.normal, 0.0f), world);

    return output;
}