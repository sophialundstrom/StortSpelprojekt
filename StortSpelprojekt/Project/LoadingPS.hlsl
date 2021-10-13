Texture2D background : register(t0);
SamplerState mySampler : register(s0);

struct Input
{
    float4 position : SV_Position;
    float2 UV : UV;
};

float4 main(Input input) : SV_TARGET
{
    return float4(background.Sample(mySampler, input.UV).xyz, 1.f);
}