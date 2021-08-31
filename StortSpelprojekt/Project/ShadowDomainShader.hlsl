Texture2D displacementTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct DS_ELEMENTS
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

cbuffer DISPLACEMENT : register(b0)
{
    float displacementFactor;
}

cbuffer MATRICES : register(b1)
{
    float4x4 world;
    float4x4 WVP;
}

#define NUM_CONTROL_POINTS 3
[domain("tri")]
DS_ELEMENTS main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<DS_ELEMENTS, NUM_CONTROL_POINTS> points)
{
    DS_ELEMENTS output;
	
    output.position = points[0].position * domain.x + points[1].position * domain.y + points[2].position * domain.z;
    output.texCoords = points[0].texCoords * domain.x + points[1].texCoords * domain.y + points[2].texCoords * domain.z;
    output.normal = points[0].normal * domain.x + points[1].normal * domain.y + points[2].normal * domain.z;

	//TRANSFORM NORMAL
    output.normal = normalize(mul(float4(output.normal, 0.0f), world)).xyz;
	
	//DISPLACEMENT
    float h = displacementTexture.SampleLevel(wrapSampler, output.texCoords, 0).r * displacementFactor;

    output.position.xyz += h * output.normal;

	//TRANSFORM FINAL POSITION
    output.position = mul(output.position, WVP);

    return output;
}