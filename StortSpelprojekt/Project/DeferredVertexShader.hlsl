struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

VS_OUTPUT main(float3 position : POSITION, float2 texCoords : TEXTURECOORDS)
{
    VS_OUTPUT output;
    
    output.position = float4(position, 1.0f);
    output.texCoords = texCoords;
    
    return output;
}