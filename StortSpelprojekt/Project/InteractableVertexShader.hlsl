cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
}

cbuffer TIME : register(b1)
{
    float time;
}

cbuffer ID : register(b2)
{
    uint ID;
}

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

VS_OUTPUT main(float3 position : POSITION)
{
    VS_OUTPUT output;
    
    output.position = float4(position, 1.0f);
    output.position.y += sin(2.0f * time + ID * 3.14159265359f / 3.0f) * 0.25f;
    output.position = mul(mul(output.position, world), viewPerspective);
    
    output.texCoords = float2(position.x + 0.5f, position.z + 0.5f);
    
    return output;
}