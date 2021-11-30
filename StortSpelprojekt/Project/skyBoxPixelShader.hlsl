TextureCube dayTexture : register(t0);
TextureCube nightTexture : register(t1);
SamplerState sam : register(s0);

float4 main(float3 worldPos : Position) : SV_TARGET
{
    //RGB IS INVERTED FOR SOME REASON
    return float4(dayTexture.Sample(sam, worldPos).bgr, 1);
}