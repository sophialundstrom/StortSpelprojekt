struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = mul(mul(float4(input.position, 1.0f), world), viewPerspective);

    return output;
}