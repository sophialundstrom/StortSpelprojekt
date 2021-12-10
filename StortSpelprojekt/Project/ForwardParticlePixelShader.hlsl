Texture2D diffuseTexture[3] : register(t0);

SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float lifeTime : LIFETIME;
    int useAlpha : USEALPHA;
    int useOpacity : USEOPACITY;
    
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 opacityColor;
    float4 blendedTex;

    color1 = diffuseTexture[0].Sample(wrapSampler, input.texCoords);
    color2 = diffuseTexture[1].Sample(wrapSampler, input.texCoords);
    opacityColor = diffuseTexture[2].Sample(wrapSampler, input.texCoords);
    
    if (input.useAlpha == 1 && input.useOpacity == 1)
    {
        blendedTex = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, opacityColor.r * 1.0f - tan(input.lifeTime));  
    }
    
    else if (input.useAlpha != 1 && input.useOpacity == 1)
    {
        blendedTex = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, opacityColor.r);   
    }
    
    else if (input.useAlpha == 1 && input.useOpacity != 1)
    {
        blendedTex = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, 1.0f - tan(input.lifeTime));
    }
    else if (input.useAlpha != 1 && input.useOpacity != 1)
    {
        blendedTex = float4(lerp(color1, color2, tan(input.lifeTime * 1.1f)).xyz, 1.0f);
    }
    
    return blendedTex;
}