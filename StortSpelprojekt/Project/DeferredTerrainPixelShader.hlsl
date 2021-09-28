// Array of textures since we are also blending based on the height value of the displacement
Texture2D blendTexture : register(t0);

Texture2D textures[3] : register(t1);

sampler wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

struct PS_OUTPUT
{
	float4 diffuseTexture : SV_TARGET0;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    const float4 blendValue = blendTexture.Sample(wrapSampler, input.texCoords);
    
    const float2 newTex = input.texCoords * 30.0f;
    
    const float4 t1 = textures[0].Sample(wrapSampler, newTex) * blendValue.x;
    const float4 t2 = textures[1].Sample(wrapSampler, newTex) * blendValue.y;
    const float4 t3 = textures[2].Sample(wrapSampler, newTex) * blendValue.z;
    
    output.diffuseTexture = t1 + t2 + t3;

	return output;
}