Texture2D diffuseTexture : register(t0);
sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    //float3 color : COLOR;
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

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    output.diffuseTexture = diffuseTexture.Sample(wrapSampler, input.texCoords);
    
    const float4 UNDEF = float4(-1.0f, -1.0f, -1.0f, 1.0f);
    output.normal =
    output.worldPosition =
    output.diffuse =
    output.ambient =
    output.specular =
    output.lightClipPosition = UNDEF;
    
    return output;
}