struct Input
{
    float4 position : POSITION;
    float2 UV : UV;
};

struct Output
{
    float4 position : SV_Position;
    float2 UV : UV;
};

Output main( Input input )
{
    Output output;
    output = input;
	return output;
}