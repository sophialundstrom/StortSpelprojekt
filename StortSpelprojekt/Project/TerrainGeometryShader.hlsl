struct GSInput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXTURECOORDS;
	float3 worldPosition : WORLDPOSITION;
	float4 lightClipPosition : LIGHTPOSITION;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXTURECOORDS;
	float3 normal : NORMAL;
    float3 worldPosition : WORLDPOSITION;
	float4 lightClipPosition : LIGHTPOSITION;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3] : SV_POSITION, inout TriangleStream<GSOutput> output)
{
	for (uint i = 0; i < 3; i++)
	{
        GSOutput vertex;
        vertex.position = input[i].position;
        vertex.texCoords = input[i].texCoords;
        vertex.worldPosition = input[i].worldPosition;
        vertex.normal = normalize(cross(input[1].worldPosition - input[0].worldPosition, input[2].worldPosition - input[0].worldPosition).xyz);
        //vertex.normal = 1.f;
		vertex.lightClipPosition = input[i].lightClipPosition;
		output.Append(vertex);
	}
}