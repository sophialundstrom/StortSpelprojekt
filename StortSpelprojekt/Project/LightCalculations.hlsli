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

float4 LightCalculation(float4 P, float3 N, float4 D, float4 S, float4 A, DirectionalLight dirLight, float3 cameraPosition)
{
    const LightResult dlResult = DirectionalLightCalculation(P, N, D, S, dirLight.lightColor, dirLight.lightDirection, cameraPosition);
    const float4 globalAmbient = 0.2f;

    return float4(dlResult.diffuse + dlResult.specular * dlResult.color) + globalAmbient + A;
}