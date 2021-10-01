struct PS_INPUT
{
    float4 position : SV_POSITION;
    uint id : ID;
};

uint4 main(PS_INPUT input) : SV_TARGET
{
    return uint4(input.id, 0, 0, 1);
}