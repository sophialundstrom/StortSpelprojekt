struct VS_INPUT
{
    float3 position : POSITION;
    float3 direction : DIRECTION;
    float lifetime : LIFETIME;
    float velocity : VELOCITY;
    int rotationDir : ROTATIONDIR;
    float rotationSpeed : ROTATIONSPEED;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float lifetime : LIFETIME;
    int rotationDir : ROTATIONDIR;
    float rotationSpeed : ROTATIONSPEED;
};

cbuffer Lifetime : register(b0)
{
    float maxLifetime;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.lifetime = (input.lifetime) / (maxLifetime);
    output.position = float4(input.position, 1.0f);
    output.rotationDir = input.rotationDir;
    output.rotationSpeed = input.rotationSpeed;
    
    return output;
}