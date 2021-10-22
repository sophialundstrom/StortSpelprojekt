cbuffer MATRICES : register(b0)
{
    float4x4 world;
    float4x4 viewPerspective;
}

float4 main(float3 pos : POSITION) : SV_POSITION
{
    return mul(mul(float4(pos, 1.0f), world), viewPerspective);
}