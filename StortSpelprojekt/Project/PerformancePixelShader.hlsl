struct PS_INPUT
{
    float4 position : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = (1.0f, 1.0f, 1.0f, 1.0f);
    float4 OK = (0.0f, 0.0f, 1.0f, 1.0f);
    float4 NOT = (1.0f, 0.0f, 0.0f, 1.0f);
    return finalColor;
}