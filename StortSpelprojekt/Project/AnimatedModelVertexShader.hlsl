struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 weights : WEIGHTS;
    int4 boneIDs : BONEIDS;
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

    float3 p = float3(0, 0, 0);
    float3 n = float3(0, 0, 0);
    for (uint i = 0; i < 4; ++i)
    {
        if (input.boneIDs[i] == -1)
            continue;
        
        p += mul(float4(input.position, 1.0), boneMatrices[input.boneIDs[i]] * input.weights[i]).xyz;
        n += mul(float4(input.normal, 0.0f), boneMatrices[input.boneIDs[i]] * input.weights[i]).xyz;
    }
         
    float4 position = float4(p, 1.0f);
    
    output.worldPosition = mul(position, world);
    
    output.position = mul(output.worldPosition, viewPerspective);

    output.lightClipPosition = mul(output.worldPosition, lightViewPerspective);
    
    output.texCoords = input.texCoords;
    
    output.normal = mul(float4(n, 0.0f), world).xyz;

    return output;
}