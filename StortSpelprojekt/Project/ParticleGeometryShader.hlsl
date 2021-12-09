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
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float lifetime : LIFETIME;
    
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
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f }
        
    };

    if (input[0].rotationDir != 0)
    {
        for (int i = 0; i < 4; i++)
        {
            //corners[i] += float4(corners[i].xyz * cos(input[0].lifetime * input[0].rotationSpeed * input[0].rotationDir)
            //        + cross(lookAt, corners[i].xyz) * sin(input[0].lifetime * input[0].rotationSpeed * input[0].rotationDir)
            //        + lookAt * dot(lookAt, corners[i].xyz) * (1 - cos(input[0].lifetime * input[0].rotationSpeed * input[0].rotationDir)), 1.0f);
            
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

    //output.texCoords = mul(corners[0], texCoords[0]);
        output.texCoords = texCoords[i];

       // output.color = input[0].color;
        outputStream.Append(output);
    }
        
  
}