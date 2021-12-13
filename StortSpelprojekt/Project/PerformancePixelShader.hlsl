struct PS_INPUT
{
    float4 position : SV_POSITION;
};

cbuffer MODELTIME : register(b1)
{
    float modeltime;
}

cbuffer LIMIT : register(b2)
{
    float limit;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 OK = float4(0.0f, 1.0f, 0.0f, 1.0f);
    float4 NOT = float4(1.0f, 0.0f, 0.0f, 1.0f);
    float4 color = NOT;

    if (modeltime < limit)
    {
        color = OK;
    }
    return color;
}