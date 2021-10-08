struct PS_INPUT
{
    float4 position : SV_POSITION;
    uint id : ID;
};

uint main(PS_INPUT input) : SV_TARGET
{
    return uint(input.id);
}