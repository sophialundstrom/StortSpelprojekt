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

cbuffer TIME : register(b1)
{
    float time;
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
    
    const int amplitude = 6;
    const float multiplier = 2.0f;
    const float PI = 3.14159265359f;
    
    if (round(output.position.x) % 2 == 0 && round(output.position.z) % 3 == 0)
        output.position.y += sin(time * multiplier) * amplitude;
    
    else if (round(output.position.x) % 3 == 0 && round(output.position.z) % 2 == 0)
        output.position.y += sin(time * multiplier + PI) * amplitude;
    
    else
        output.position.y += sin(time * multiplier + PI / 2.0f) * amplitude / 2.0f;

	output.worldPosition = output.position.xyz;

    //TRANSFORM FINAL POSITION
	output.position = mul(output.position, viewPerspective);

	return output;
}