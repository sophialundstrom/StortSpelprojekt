//TextureCube tex : register(t0);
//SamplerState sam : register(s0);

float4 main(float3 worldPos : Position) : SV_TARGET
{
    return float4(0, -worldPos.y + 0.7, 0.5, 1.0f);
}