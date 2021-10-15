cbuffer CAMERA : register(b0)
{
    float3 cameraPosition;
}

cbuffer MATRIX : register(b1)
{
    float4x4 viewPerspective;
}

cbuffer EXTENTS : register(b2)
{
    float2 extents;
}

struct GS_INPUT
{
    float4 position : SV_POSITION;
   // float3 color : COLOR;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
   // float3 color : COLOR;
};

[maxvertexcount(4)]
void main(
    point GS_INPUT input[1],
    inout TriangleStream<GS_OUTPUT> outputStream)
{
    const float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 lookAt = cameraPosition - input[0].position.xyz;

    lookAt.y = 0.0f;
    lookAt = normalize(lookAt);
    float3 right = cross(up, lookAt);

    float4 corners[] =
    {
        { input[0].position.xyz + extents.x * right + extents.y * up, 1.0f },
        { input[0].position.xyz - extents.x * right + extents.y * up, 1.0f },
        { input[0].position.xyz + extents.x * right - extents.y * up, 1.0f },
        { input[0].position.xyz - extents.x * right - extents.y * up, 1.0f }
    };

    float2 texCoords[] =
    {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f }
        
    };

    
    for (int i = 0; i < 4; ++i)
    {
        GS_OUTPUT output;
        output.position = mul(corners[i], viewPerspective);

    //output.texCoords = mul(corners[0], texCoords[0]);
        output.texCoords = texCoords[i];

       // output.color = input[0].color;
        outputStream.Append(output);
    }
    
  
    
  
}