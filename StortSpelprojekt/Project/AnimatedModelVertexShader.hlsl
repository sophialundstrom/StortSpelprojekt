struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 weights : WEIGHTS;
    uint4 boneIDs : BONEIDS;
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

StructuredBuffer<float4x4> boneMatrices : register(t0);

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4x4 boneTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    
    for (uint i = 0; i < 4; ++i)
        boneTransform += boneMatrices[input.boneIDs[i]] * input.weights[i];
    
    float4 position = float4(input.position, 1.0f);
    
    position = mul(position, boneTransform);
    
    output.worldPosition = mul(position, world);
    
    output.position = mul(output.worldPosition, viewPerspective);

    output.lightClipPosition = mul(output.worldPosition, lightViewPerspective);
    
    output.texCoords = input.texCoords;

    float4 normal = float4(input.normal, 0.0f);
    output.normal = mul(mul(normal, boneTransform), world).xyz;

    return output;
}