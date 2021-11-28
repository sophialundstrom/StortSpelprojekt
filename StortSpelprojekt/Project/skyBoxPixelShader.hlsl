//TextureCube tex : register(t0);
//SamplerState sam : register(s0);

float4 main(float3 worldPos : Position) : SV_TARGET
{
    return float4(worldPos.x, worldPos.y, worldPos.z, 1.f);
}