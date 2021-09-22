Texture2D displacementTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct DS_ELEMENTS
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

cbuffer MATRIX : register(b0)
{
    float4x4 viewPerspective;
}

#define NUM_CONTROL_POINTS 3
[domain("tri")]
DS_ELEMENTS main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<DS_ELEMENTS, NUM_CONTROL_POINTS> patch)
{
    DS_ELEMENTS output;
	
    //BARYCENTRIC COORDINATES
    output.position = patch[0].position * domain.x + patch[1].position * domain.y + patch[2].position * domain.z;
    output.texCoords = patch[0].texCoords * domain.x + patch[1].texCoords * domain.y + patch[2].texCoords * domain.z;
    
	//DISPLACEMENT
    float h = displacementTexture.SampleLevel(wrapSampler, output.texCoords, 0).r;

    output.position.xyz += h * float3(0.0f, 3.0f, 0.0f);

	//TRANSFORM FINAL POSITION
    output.position = mul(output.position, viewPerspective);

    return output;
}