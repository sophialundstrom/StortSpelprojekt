struct GSInput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXTURECOORDS;
	//float3 normal : NORMAL;
	//float3 worldPos : WORLDPOSITION;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXTURECOORDS;
	//float3 normal : NORMAL;
	//float3 worldPos : WORLDPOSITION;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3] : SV_POSITION, inout TriangleStream<GSOutput> output)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput vertex = input[i];
		//vertex.normal = normalize(cross(input[1].worldPos - input[0].worldPos, input[2].worldPos - input[0].worldPos).xyz);
		output.Append(vertex);
	}
}