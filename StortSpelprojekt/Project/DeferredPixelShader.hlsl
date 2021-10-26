Texture2D diffuseTextures : register(t0);
Texture2D normals : register(t1);
Texture2D worldPositions : register(t2);
Texture2D diffuse : register(t3);
Texture2D ambient : register(t4);
Texture2D specular : register(t5);
Texture2D lightClipPositions : register(t6);

Texture2D shadowMap : register(t7);

SamplerState wrapSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXTURECOORDS;
};

cbuffer DIRECTIONAL_LIGHT : register(b0)
{
    float4 lightColor;
    float3 lightDirection;
}

cbuffer CAMERA : register(b1)
{
    float3 cameraPosition;
}

cbuffer NUM_LIGHTS : register(b2)
{
    uint numLights;
}

struct POINT_LIGHT
{
    float4 color;
    float3 position;
    float range;
    float3 attenuation;
};
StructuredBuffer<POINT_LIGHT> lights : register(t8);

struct LightResult
{
    float4 diffuse;
    float4 specular;
    float4 color;
};

LightResult PointLightCalculation(float4 P, float3 N, float4 D, float4 S)
{
    LightResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 } };
    float4 V = { 1, 1, 1, 1 };

    for (uint i = 0; i < numLights; ++i)
    {
        const POINT_LIGHT pointLight = lights[i];
        
        float3 L = pointLight.position - P.xyz;
        float dist = length(L);
        L /= dist;
        
        if (dist > pointLight.range)
            continue;
        
        float attenuation = 1.0f / (pointLight.attenuation[0] + dist * pointLight.attenuation[1] + dist * dist * pointLight.attenuation[2]);
	
        float diffuse = dot(N, L);

        if (diffuse < 0.0f) // IF BACKFACED
            return result;
	
        if (D.x > 0)
            result.diffuse += saturate(diffuse) * D * attenuation;
        else
            result.diffuse += saturate(diffuse) * V * attenuation;
        
        float3 R = normalize(reflect(-L, N));
        float3 E = normalize(cameraPosition - P.xyz);
        float specular = pow(saturate(dot(R, E)), S.w);

        if (S.x > 0)
            result.specular += specular * S * attenuation;
        else
            result.specular += specular * V * attenuation;

        result.color *= pointLight.color;
    }
    
    result.diffuse = saturate(result.diffuse);
    result.specular = saturate(result.specular);
    result.color = saturate(result.color);
    
    return result;
}

LightResult DirectionalLightCalculation(float4 P, float3 N, float4 D, float4 S)
{
    LightResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 } };
    float4 V = { 1, 1, 1, 1 };

    float3 E = normalize(cameraPosition - P.xyz);
	
    float diffuse = dot(N, -lightDirection);

    if (diffuse < 0.0f) // IF BACKFACED
        return result;
	
    if (D.x > 0)
        result.diffuse = saturate(diffuse * D);
    else
        result.diffuse = saturate(diffuse * V);

    float3 R = normalize(reflect(lightDirection, N));
	
    float specular = pow(saturate(dot(R, E)), S.w);

    if (S.x > 0)
        result.specular = saturate(specular * S);
    else
        result.specular = saturate(specular * V);

    result.color = lightColor;
    
    return result;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    const float4 backgroundColor = { 117.0f / 255.0f, 141.0f / 255.0f, 156.0f / 255.0f, 1.0f };
    const float globalAmbient = 0.2f;
    
    const float4 T = diffuseTextures.Sample(wrapSampler, input.texCoords);
    
    const float3 N = normals.Sample(wrapSampler, input.texCoords).xyz;
    if (length(N.xyz) == 0)
        return backgroundColor * globalAmbient; // IF NORMAL IS 0 THERE IS NO PRIMITIVE (BACKGROUND SHOULD NOT BE SHADED)
	
    const float4 P = worldPositions.Sample(wrapSampler, input.texCoords);
    const float4 D = diffuse.Sample(wrapSampler, input.texCoords);
    const float4 A = ambient.Sample(wrapSampler, input.texCoords);
    const float4 S = specular.Sample(wrapSampler, input.texCoords);
    float4 L = lightClipPositions.Sample(wrapSampler, input.texCoords);

    if (S.x < 0) //PARTICLE SYSTEM DOESNT NEED SHADING
        return T;
	
	//SHADOWS
    L.xyz /= L.w; //PERSPECTIVE DIVIDE (NDC-COORDS)
    const float2 tx = float2(0.5f * L.x + 0.5f, -0.5f * L.y + 0.5f); // [-1,1] => [0, 1]
    const float sm = shadowMap.Sample(wrapSampler, tx).r;
    const float shadow = (sm + 0.005 < L.z) ? 0.0f : 1.0f; //if closest depth (sample) < pixel-depth there is a primitive in front castings shadow.
	
	//LIGHT
    const LightResult dlResult = DirectionalLightCalculation(P, N, D, S);
    const LightResult plResult = PointLightCalculation(P, N, D, S);

    const float4 finalLighting = float4(dlResult.diffuse + dlResult.specular * dlResult.color) * shadow + globalAmbient + A
                                /*+ float4(plResult.diffuse + plResult.specular * plResult.color)*/;
	//RESULT
    const float4 finalColor = T * (saturate(finalLighting));
	
    return T;
}