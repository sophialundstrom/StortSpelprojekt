struct VS_INPUT
{
    float3 position : POSITION;
    float3 direction : DIRECTION;
    //float2 texCoords : TEXTURECOORDS;
    float lifetime : LIFETIME;
    float velocity : VELOCITY;
};



struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    //float2 texCoords : TEXTURECOORDS;
    float lifetime : LIFETIME;
    //float3 color : COLOR;
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
   // output.color = input.color;
    //output.texCoords = input.texCoords;
    
    return output;
}