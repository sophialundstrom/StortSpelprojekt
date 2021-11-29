Texture2D displacementTexture : register(t0);
SamplerState wrapSampler : register(s0);

float3 Wave(float2 direction, float amplitude, float multiplier, float4 position, float time, float speed)
{
    float PI = 3.14159265359f;
    float k = (2 * PI) / multiplier;
    float phaseSpeed = speed;
    float2 d = normalize(direction);
    float f = k * (dot(direction, position.xz) - phaseSpeed * time);

    return float3(d.x * (amplitude * cos(f)), amplitude * sin(f), d.y * (amplitude * cos(f)));
}

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

    output.position.xyz += Wave(float2(1, 1), 2.0f, 0.5f, output.position, time, 0.25f);
    //output.position.xyz += Wave(float2(1, -1), 1.0f, 0.1f, output.position, time, 0.25f);

    //RECALCULATE NORMAL, TANGENT AND BINORMAL
    //output.tangent = normalize(float3(1, k * amplitude * cos(f), 0));
    //output.normal = float3(-output.tangent.y, output.tangent.x, 0);

    output.worldPosition = output.position.xyz;

    //TRANSFORM FINAL POSITION
	output.position = mul(output.position, viewPerspective);

	return output;
}