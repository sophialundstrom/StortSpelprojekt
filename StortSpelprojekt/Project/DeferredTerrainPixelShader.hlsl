// Array of textures since we are also blending based on the height value of the displacement
Texture2D diffuseTexture : register(t0);

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

    output.diffuseTexture = diffuseTexture.Sample(wrapSampler, input.texCoords);

	return output;
}