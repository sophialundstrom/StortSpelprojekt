struct GSInput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXTURECOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 biNormal : BINORMAL;
	float3 worldPosition : WORLDPOSITION;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXTURECOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 biNormal : BINORMAL;
	float3 worldPosition : WORLDPOSITION;
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
		vertex.normal = input[i].normal;
		vertex.tangent = input[i].tangent;
		vertex.biNormal = input[i].biNormal;
		output.Append(vertex);
	}
}