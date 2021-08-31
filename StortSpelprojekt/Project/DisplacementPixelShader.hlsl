Texture2D diffuseTextures[2] : register(t0);
SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
    float blendValue : BLENDVALUE;
};

struct PS_OUTPUT
{
    float4 diffuseTexture : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 worldPosition : SV_TARGET2;
    float4 diffuse : SV_TARGET3;
    float4 ambient : SV_TARGET4;
    float4 specular : SV_TARGET5;
    float4 lightClipPosition : SV_TARGET6;
};

cbuffer Material : register(b0)
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float specularPower;
};

cbuffer Blending : register(b1)
{
    float blendingFactor;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    //HEIGHT DEPENDENT TEXTURE BLENDING
    float4 lowerTex = diffuseTextures[0].Sample(wrapSampler, input.texCoords);
    float4 higherTex = diffuseTextures[1].Sample(wrapSampler, input.texCoords);
    
    output.diffuseTexture = lerp(lowerTex, higherTex, input.blendValue * blendingFactor);

    //NORMAL + WORLD POSITION + LIGHT CLIP SPACE
    output.normal = float4(input.normal, 1.0f);
    output.worldPosition = input.worldPosition;
    output.lightClipPosition = input.lightClipPosition;

    //MATERIAL
    output.diffuse = diffuse;
    output.ambient = ambient;
    output.specular = specular;
    output.specular.w = specularPower;
        
    return output;
}