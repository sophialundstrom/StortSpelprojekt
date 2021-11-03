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

float ShadowCalculation(float4 LCP, Texture2D shadowMap, SamplerState wrapSampler)
{
    LCP.xyz /= LCP.w; //PERSPECTIVE DIVIDE (NDC-COORDS)
    const float2 tx = float2(0.5f * LCP.x + 0.5f, -0.5f * LCP.y + 0.5f); // [-1,1] => [0, 1]
    const float sm = shadowMap.Sample(wrapSampler, tx).r;
    float shadow = (sm + 0.005 < LCP.z) ? 0.0f : 1.0f; //if closest depth (sample) < pixel-depth there is a primitive in front castings shadow.

    if (tx.x > 1.0f || tx.x < 0.0f ||
        tx.y > 1.0f || tx.y < 0.0f ||
        LCP.z > 1.0f || LCP.z < 0.0f)
        shadow = 1.0f;

    return shadow;
}

float4 LightCalculation(float4 LCP, float4 P, float3 N, float4 D, float4 S, float4 A, DirectionalLight dirLight, float3 cameraPosition, Texture2D shadowMap, SamplerState wrapSampler)
{
    const LightResult dlResult = DirectionalLightCalculation(P, N, D, S, dirLight.lightColor, dirLight.lightDirection, cameraPosition);
    const float shadow = ShadowCalculation(LCP, shadowMap, wrapSampler);
    const float4 globalAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };

    return float4(dlResult.diffuse/* + dlResult.specular * dlResult.color*/)/* * shadow*/ + A;
}