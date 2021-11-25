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
    float3 biNormal : BINORMAL;
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
    output.biNormal = float3(0, 0, 1);
    
    const int steepness = 1.0f;
    const float multiplier = 20.0f;
    const float PI = 3.14159265359f;
    float k = 2 * PI / multiplier;
    float phaseSpeed = sqrt(9.8 / k);
    float2 direction = normalize(float2(-1, -1));
    float f = k * (dot(direction, output.position.xz) - phaseSpeed * time);
    float a = (steepness / k);
    //RECALCULATE NORMALS OF WAVES
    //if (round(output.position.x) % 2 == 0 && round(output.position.z) % 3 == 0)
    //    output.position.y += amplitude * sin((2 * PI * multiplier * time) + theta);
    //else if (round(output.position.x) % 3 == 0 && round(output.position.z) % 2 == 0)
    //    output.position.y += amplitude * sin((2 * PI * multiplier * time) + theta + 0.2f);
    //else
    //    output.position.y += amplitude * cos((2 * PI * multiplier * time) + theta + 0.5f);

    output.position.x += direction.x * (a * cos(f));
    output.position.y = a * sin(f);
    output.position.z += direction.y * (a * cos(f));

    //RECALCULATE NORMAL, TANGENT AND BINORMAL
    output.tangent = float3(1 + direction.x * direction.x * (steepness * sin(f)), direction.x * (steepness * cos(f)), direction.x * direction.y * (steepness * sin(f)));
    output.biNormal = float3(direction.x * direction.y * (steepness * sin(f)), direction.y * (steepness * cos(f)), 1 + direction.y * direction.y * (steepness * sin(f)));
    output.normal = normalize(float3(cross(output.biNormal, output.tangent)));

    output.worldPosition = output.position.xyz;

    //TRANSFORM FINAL POSITION
	output.position = mul(output.position, viewPerspective);

	return output;
}