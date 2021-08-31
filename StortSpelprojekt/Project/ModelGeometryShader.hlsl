struct GS_ELEMENTS
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
    float4 worldPosition : WORLDPOSITION;
    float4 lightClipPosition : LIGHTPOSITION;
};

[maxvertexcount(3)]
void main(triangle GS_ELEMENTS input[3], inout TriangleStream<GS_ELEMENTS> output)
{
    float3 p0 = input[0].position.xyz;
    float3 p1 = input[1].position.xyz;
    float3 p2 = input[2].position.xyz;

    //Calc normal from vertices
    float3 faceNormal = normalize(cross(p1 - p0, p2 - p0));

    //Get dot value, if value >= 0.0f the triangle is frontfaced. (p0 = (0,0,0) - (p0), a vector from pov to point).
    float dotVal = dot(faceNormal, -p0);

    if (dotVal >= 0.0f)
        for (uint i = 0; i < 3; i++)
            output.Append(input[i]);
}