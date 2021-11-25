Texture2D displacementTexture : register(t0);
SamplerState wrapSampler : register(s0);

struct DS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct DS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
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

cbuffer thetaOffset : register(b2)
{
    float theta;
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
    output.tangent = patch[0].tangent * domain.x + patch[1].tangent * domain.y + patch[2].tangent * domain.z;
    output.normal = patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z;
    
    const int amplitude = 2.0f;
    const float multiplier = 100.0f;
    const float PI = 3.14159265359f;
    float k = (2 * PI) / multiplier;
    float phaseSpeed = sqrt(9.8f / k);
    float2 direction = normalize(float2(-1, -1));
    float f = k * (dot(direction, output.position.xz) - phaseSpeed * time);

    output.position.x += direction.x * (amplitude * cos(f));
    output.position.y = amplitude * sin(f);
    output.position.z += direction.y * (amplitude * cos(f));

    //RECALCULATE NORMAL, TANGENT AND BINORMAL
    //output.tangent = float3(1 - direction.x * direction.x * (amplitude * sin(f)), direction.x * (amplitude * cos(f)), -direction.x * direction.y * (amplitude * sin(f)));
    //output.biNormal = float3(-direction.x * direction.y * (amplitude * sin(f)), direction.y * (amplitude * cos(f)), 1 - direction.y * direction.y * (amplitude * sin(f)));
    //output.normal = normalize(float3(cross(output.biNormal, output.tangent)));

    output.worldPosition = output.position.xyz;

    //TRANSFORM FINAL POSITION
	output.position = mul(output.position, viewPerspective);

	return output;
}