Texture2D displacementTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct DS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
    float blendValue : BLENDVALUE;
};

struct HS_OUTPUT
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
    float4x4 viewPerspective;
    float4x4 lightViewPerspective;
}

#define NUM_CONTROL_POINTS 3
[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_OUTPUT, NUM_CONTROL_POINTS> patch)
{
    DS_OUTPUT output;
	
    //BARYCENTRIC COORDINATES => domain.x + domain.y + domain.z = 1.0f
    output.position = patch[0].position * domain.x + patch[1].position * domain.y + patch[2].position * domain.z;
    output.texCoords = patch[0].texCoords * domain.x + patch[1].texCoords * domain.y + patch[2].texCoords * domain.z;
    output.normal = patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z;

	//TRANSFORM NORMAL
    output.normal = normalize(mul(float4(output.normal, 0.0f), world)).xyz;
	
	//DISPLACEMENT
    float h = displacementTexture.SampleLevel(wrapSampler, output.texCoords, 0).r * displacementFactor;
    output.blendValue = h;

    output.position.xyz += h * output.normal;

	//TRANSFORM FINAL POSITION
    output.worldPosition = mul(output.position, world);
    output.position = mul(output.worldPosition, viewPerspective);
    output.lightClipPosition = mul(output.worldPosition, lightViewPerspective);

    return output;
}