Texture2D displacementTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct DS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

struct DS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 worldPosition : WORLDPOSITION;
    float3 lightClipPosition : LIGHT;
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

cbuffer MATRICES : register(b1)
{
    float4x4 world;
    float4x4 x;
    float4x4 lightViewPerspective;
}

#define NUM_CONTROL_POINTS 3
[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<DS_INPUT, NUM_CONTROL_POINTS> patch)
{
    DS_OUTPUT output;
	
    //BARYCENTRIC COORDINATES
    output.position = patch[0].position * domain.x + patch[1].position * domain.y + patch[2].position * domain.z;
    output.texCoords = patch[0].texCoords * domain.x + patch[1].texCoords * domain.y + patch[2].texCoords * domain.z;
    
	//DISPLACEMENT
    float h = displacementTexture.SampleLevel(wrapSampler, output.texCoords, 0).r * 255 * 2.0f - 100.0f;

    output.position.y = h;
    
    output.worldPosition = output.position.xyz;

	//TRANSFORM FINAL POSITION
    output.position = mul(output.position, viewPerspective);

    output.lightClipPosition = mul(output.worldPosition, lightViewPerspective);

    return output;
}