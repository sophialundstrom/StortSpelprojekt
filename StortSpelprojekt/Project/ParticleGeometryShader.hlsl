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
    float lifetime : LIFETIME;
    int rotationDir : ROTATIONDIR;
    float rotationSpeed : ROTATIONSPEED;
    int useAlpha : USEALPHA;
    int useOpacity : USEOPACITY;
    float scaleOverTime : SCALEOVERTIME;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float lifetime : LIFETIME;
    int useAlpha : USEALPHA;
    int useOpacity : USEOPACITY;
};

[maxvertexcount(4)]
void main(
    point GS_INPUT input[1],
    inout TriangleStream<GS_OUTPUT> outputStream)
{
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 lookAt = cameraPosition - input[0].position.xyz;

    //lookAt.y = 0.0f;
    lookAt = normalize(lookAt);
    float3 right = cross(up, lookAt);
    right = normalize(right);
    up = cross(lookAt, right);
    up = normalize(up);
    
    float scale;
    
    if (input[0].scaleOverTime > 0.0f)
    {
         scale = input[0].scaleOverTime * input[0].lifetime;
    }
    
    else
    {
        scale = input[0].scaleOverTime + (1 - input[0].lifetime);
    }
    
    float2 scaledExtents = scale * extents;
   
    float4 corners[] =
    {
        { input[0].position.xyz + scaledExtents.x * right + scaledExtents.y * up, 1.0f },
        { input[0].position.xyz - scaledExtents.x * right + scaledExtents.y * up, 1.0f },
        { input[0].position.xyz + scaledExtents.x * right - scaledExtents.y * up, 1.0f },
        { input[0].position.xyz - scaledExtents.x * right - scaledExtents.y * up, 1.0f }
    };

    float2 texCoords[] =
    {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f }
    };

    if (input[0].rotationDir != 0)
    {
        for (int i = 0; i < 4; i++)
        {
            float3 parallel = dot(lookAt, corners[i].xyz) * lookAt;
            float3 projection = corners[i].xyz - parallel;
            corners[i] = float4(parallel + cos(input[0].lifetime * input[0].rotationSpeed * input[0].rotationDir) * projection +
                         sin(input[0].lifetime * input[0].rotationSpeed * input[0].rotationDir) * cross(lookAt, corners[i].xyz), 1.0f);   
        }
    }
        
    GS_OUTPUT output;
    output.lifetime = input[0].lifetime;
    for (int i = 0; i < 4; ++i)
    {
        output.position = mul(corners[i], viewPerspective);
        output.texCoords = texCoords[i];
        output.useAlpha = input[0].useAlpha;
        output.useOpacity = input[0].useOpacity;
        outputStream.Append(output);
    }
}