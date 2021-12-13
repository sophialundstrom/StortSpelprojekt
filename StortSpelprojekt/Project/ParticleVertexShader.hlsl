struct VS_INPUT
{
    float3 position : POSITION;
    float3 direction : DIRECTION;
    float lifetime : LIFETIME;
    float velocity : VELOCITY;
    int rotationDir : ROTATIONDIR;
    float rotationSpeed : ROTATIONSPEED;
    int useAlpha : USEALPHA;
    int useOpacity : USEOPACITY;
    float scaleOverTime : SCALEOVERTIME;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float lifetime : LIFETIME;
    int rotationDir : ROTATIONDIR;
    float rotationSpeed : ROTATIONSPEED;
    int useAlpha : USEALPHA;
    int useOpacity : USEOPACITY;
    float scaleOverTime : SCALEOVERTIME;
    
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
    output.useAlpha = input.useAlpha;
    output.useOpacity = input.useOpacity;
    output.scaleOverTime = input.scaleOverTime;
    return output;
}