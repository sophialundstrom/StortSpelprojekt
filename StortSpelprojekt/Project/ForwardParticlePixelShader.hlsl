Texture2D diffuseTexture[2] : register(t0);
//Texture2D diffuseTexture : register(t0);

SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
	float lifeTime : LIFETIME;
   // float3 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 finalColor;

    color1 = diffuseTexture[0].Sample(wrapSampler, input.texCoords);
    color2 = diffuseTexture[1].Sample(wrapSampler, input.texCoords);

    //finalColor = (input.lifeTime, input.lifeTime, input.lifeTime, 1);
    
    finalColor = lerp(color1, color2, input.lifeTime);
    return finalColor;
  //  return diffuseTexture.Sample(wrapSampler, input.texCoords);

}