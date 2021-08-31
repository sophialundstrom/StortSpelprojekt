struct HS_ELEMENTS
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
    float3 normal : NORMAL;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

cbuffer TESSELATION : register(b0)
{
    float tesselationAmount;
}

#define NUM_CONTROL_POINTS 3
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<HS_ELEMENTS, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    Output.EdgeTessFactor[0] =
	Output.EdgeTessFactor[1] =
	Output.EdgeTessFactor[2] =
	Output.InsideTessFactor = tesselationAmount;

    return Output;
}

[domain("tri")]
[partitioning("fractional_odd")] //ROUNDS UP TO CLOSEST ODD NUMBER (TESSFACTORS)
[outputtopology("triangle_cw")] //CLOCKWISE TRIANGLE AS OUTPUT
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_ELEMENTS main(
	InputPatch<HS_ELEMENTS, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    return ip[i];
}