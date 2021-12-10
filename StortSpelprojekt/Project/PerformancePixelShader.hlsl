struct PS_INPUT
{
    float4 position : SV_POSITION;
};

cbuffer MODELTIME : register(b1)
{
    float modeltime;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = (1.0f, 1.0f, 1.0f, 1.0f);
    float4 OK = (0.0f, 0.0f, 1.0f, 1.0f);
    float4 NOT = (1.0f, 0.0f, 0.0f, 1.0f);
    return float4(modeltime * 1000, 1.0 - modeltime * 1000, 0.0f, 1.0f);
}