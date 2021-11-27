cbuffer TransformCBuf : register(b0)
{
    float4x4 viewProj;
};

struct VSOut
{
    float3 worldPos : Position;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;

//  float4x4 identity =
//  {
//       1, 0, 0, 0,
//       0, 1, 0, 0,
//       0, 0, 1, 0,
//       0, 0, 0, 1
//  };
    

	vso.worldPos = pos;
	vso.pos = mul(float4(pos, 0.0f), viewProj);

    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
	vso.pos.z = vso.pos.w;
    return vso;
}