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
    output.normal = patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z;
    
    
    const int amplitude = 2.0f;
    const float multiplier = 30.0f;
    const float PI = 3.14159265359f;
    float k = 2 * PI / multiplier;
    float phaseSpeed = sqrt(9.8 / k);
    float2 direction = normalize(float2(-1, -1));

    //RECALCULATE NORMALS OF WAVES
    //if (round(output.position.x) % 2 == 0 && round(output.position.z) % 3 == 0)
    //    output.position.y += amplitude * sin((2 * PI * multiplier * time) + theta);
    //else if (round(output.position.x) % 3 == 0 && round(output.position.z) % 2 == 0)
    //    output.position.y += amplitude * sin((2 * PI * multiplier * time) + theta + 0.2f);
    //else
    //    output.position.y += amplitude * cos((2 * PI * multiplier * time) + theta + 0.5f);

    float f = k * (dot(direction, output.position.xz) - phaseSpeed * time);
    output.position.x += direction.x * (amplitude * cos(f));
    output.position.y = amplitude * sin(f);
    output.position.z += direction.y * (amplitude * cos(f));

    //RECALCULATE NORMAL AND TANGENT
    output.tangent = normalize(float3(1 - k * amplitude * sin(f), k * amplitude * cos(f), 0));
    output.normal = float3(-output.tangent.y, output.tangent.x, 0);

    output.worldPosition = output.position.xyz;

    //TRANSFORM FINAL POSITION
	output.position = mul(output.position, viewPerspective);

	return output;
}