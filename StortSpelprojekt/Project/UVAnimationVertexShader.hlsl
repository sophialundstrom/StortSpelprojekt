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
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
};

cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
    float4x4 lightViewPerspective;
}

cbuffer TIME : register(b1)
{
    float time;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = float4(input.position, 1.0f);
    
    output.worldPosition = mul(output.position, world);
    
    const float Xoffset = cos(5.0f * time + (input.texCoords.x * 10.0f));
    const float Yoffset = sin(5.0f * time + (input.texCoords.y * 10.0f)) + 1.0f;
    output.worldPosition.y += Yoffset * 0.1f;
    output.worldPosition.x += Xoffset * 0.1f;
    
    output.position = mul(output.worldPosition, viewPerspective);
    
    output.lightClipPosition = mul(output.worldPosition, lightViewPerspective);
    
    output.texCoords = input.texCoords;

    output.normal = mul(float4(input.normal, 0.0f), world).xyz;

    return output;
}