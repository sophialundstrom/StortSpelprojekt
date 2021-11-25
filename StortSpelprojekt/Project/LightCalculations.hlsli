struct LightResult
{
    float4 diffuse;
    float4 specular;
    float4 color;
};

struct DirectionalLight
{
    float4 lightColor;
    float3 lightDirection;
};

LightResult DirectionalLightCalculation(float4 P, float3 N, float4 D, float4 S, float4 lightColor, float3 lightDirection, float3 cameraPosition)
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

struct POINT_LIGHT
{
    float4 color;
    float3 position;
    float range;
    float3 attenuation;
    float intensity;
};

LightResult PointLightCalculation(float4 P, float3 N, float4 D, float4 S, float4 lightColor, float lightIntensity, float3 lightPosition, float lightRange, float3 lightAttenuation, float3 cameraPosition)
{
    LightResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 } };
    float4 V = { 1, 1, 1, 1 };
   
    float3 L = lightPosition - P.xyz;
    float dist = length(L);
    L /= dist;

    if (dist > lightRange)
        return result;

    float attenuation = 1.0f / (lightAttenuation[0] + (lightAttenuation[1] * dist) + (dist * dist * lightAttenuation[2]));
        
    float diffuse = dot(N, L);

    if (diffuse < 0.0f) // IF BACKFACED
        return result;

    if (D.x > 0)
        result.diffuse += saturate(diffuse) * D * attenuation;
    else
        result.diffuse += saturate(diffuse) * V * attenuation;
    
    result.diffuse *= lightIntensity;

    float3 R = normalize(reflect(-L, N));
    float3 E = normalize(cameraPosition - P.xyz);
    float specular = pow(saturate(dot(R, E)), S.w);

    if (S.x > 0)
        result.specular += specular * S * attenuation;
    else
        result.specular += specular * V * attenuation;

    result.color *= lightColor;
    
    //result.diffuse = float4(attenuation, attenuation, attenuation, 1.0f);
    
    return result;
}

float4 LightCalculation(float4 P, float3 N, float4 D, float4 S, float4 A, DirectionalLight dirLight, float3 cameraPosition)
{
    const LightResult dlResult = DirectionalLightCalculation(P, N, D, S, dirLight.lightColor, dirLight.lightDirection, cameraPosition);

    return float4((dlResult.diffuse + dlResult.specular) * dlResult.color);
}