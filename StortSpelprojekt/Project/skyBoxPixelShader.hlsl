TextureCube dayTexture : register(t0);
TextureCube nightTexture : register(t1);
SamplerState sam : register(s0);

cbuffer fadeBuffer : register(b0)
{
    float fadeSlider;
    float pad1;
    float pad2;
    float pad3;
};

float4 main(float3 worldPos : Position) : SV_TARGET
{
    
    
    //RGB IS INVERTED FOR SOME REASON
    float3 dayColor = float3(dayTexture.Sample(sam, worldPos).bgr);
    dayColor *= fadeSlider;
    float3 nightColor = float3(nightTexture.Sample(sam, worldPos).bgr);
    nightColor *= (1 - fadeSlider);
    
    return float4(dayColor + nightColor, 1);
}