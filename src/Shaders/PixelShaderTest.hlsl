cbuffer lightBuffer : register(b1)
{
    float4 lightDirection;
    float4 lightColor;
    matrix lightSpaceMatrix;
};

Texture2D shadowMap : register(t0);
Texture2D diffuseTex : register(t1);
Texture2D normalTex : register(t2);

SamplerState smplr;

float ShadowCalculation(float4 fragPosLightSpace, float3 normal, float3 lightDir)
{
    float4 projCoords = fragPosLightSpace * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 1.0f);
    
    float closestDepth = shadowMap.Sample(smplr, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    float shadow = 0.0;
    float2 texelSize = 1.0 / float2(4096, 4096);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = shadowMap.Sample(smplr, projCoords.xy + float2(x, y) * texelSize).r;
            //float pcfDepth = texture(shadowMap, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    return shadow;
}

float4 main(float3 fragPos : FragmentPosition, float3 viewPos : ViewPosition, float4 fragPosLightSpace : FragmentPositionLightSpace, float3x3 tbn : TBN, float2 texCoord : TexCoord) : SV_TARGET
{
    //sample normal
    float3 normal = normalTex.Sample(smplr, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(mul(normal, tbn));
    
    //sample texture
    float4 textureColor = diffuseTex.Sample(smplr, texCoord);
    
    //calculate light direction
    float3 lightDir = normalize(-lightDirection.xyz);
    
    //calculate diffuse light
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    float3 diffuse = diffuseStrength * lightColor;
    
    //calculate specular light
    float specularStrength = 0.5f;
    float3 viewDir = normalize(viewPos - fragPos);
    
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
    float3 specular = specularStrength * spec * lightColor;
    
    //ambient light
    float ambientStrength = 0.25f;
    float3 ambient = ambientStrength * lightColor;
    
    // calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    float3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    
    //sky light
    if (1.0 - shadow < 0.1f)
    {
        lighting += float3(0.05f, 0.07, 0.07);

    }
    
    return float4(lighting, 1.0f) * textureColor;
    //return float4(norm);

}