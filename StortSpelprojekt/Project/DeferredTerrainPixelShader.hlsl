Texture2D blendTexture : register(t0);

Texture2D textures[3] : register(t1);

sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 worldPosition : WORLDPOSITION;
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

cbuffer test : register(b0)
{
    float4x4 lightMatrix;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    const float4 blendValue = blendTexture.Sample(wrapSampler, input.texCoords);
    
    const float2 newTex = input.texCoords * 30.0f;
    
    const float4 t1 = textures[0].Sample(wrapSampler, newTex) * blendValue.x;
    const float4 t2 = textures[1].Sample(wrapSampler, newTex) * blendValue.y;
    const float4 t3 = textures[2].Sample(wrapSampler, newTex) * blendValue.z;
    
    output.diffuseTexture = t1 + t2 + t3;
    
    output.normal = float4(input.normal, 1.0f);
    
    output.worldPosition = float4(input.worldPosition, 1.0f);
    
    const float4 UNDEF = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.diffuse = output.specular = UNDEF;
    
    output.ambient = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    output.lightClipPosition = mul(float4(input.worldPosition, 1.0f), lightMatrix);
    
	return output;
}