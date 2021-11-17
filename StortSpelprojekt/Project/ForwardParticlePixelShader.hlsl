Texture2D diffuseTexture[3] : register(t0);

SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float lifeTime : LIFETIME;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 finalColor;
    float4 opacityColor;
    float4 blendedTex;
    float4 masked;

    color1 = diffuseTexture[0].Sample(wrapSampler, input.texCoords);
    color2 = diffuseTexture[1].Sample(wrapSampler, input.texCoords);
    opacityColor = diffuseTexture[2].Sample(wrapSampler, input.texCoords);
    
    blendedTex = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, opacityColor.r * 1 - tan(input.lifeTime));
    
   // finalColor = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, 1 - tan(input.lifeTime));

    
    
    //finalColor = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, 1 - tan(input.lifeTime));
    return blendedTex;

}