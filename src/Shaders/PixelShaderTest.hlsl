cbuffer lightBuffer : register(b0)
{
    float4 lightDirection;
    float4 lightColor;
    matrix lightSpaceMatrix;
};

Texture2D shadowMap : register(t0);
Texture2D diffuseTex : register(t1);

SamplerState smplr;

float ShadowCalculation(float4 fragPosLightSpace, float3 normal, float3 lightDir)
{
    float4 projCoords = fragPosLightSpace * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 1.0f);
    
    float closestDepth = shadowMap.Sample(smplr, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

float4 main(float3 fragPos : FragmentPosition, float3 viewPos : ViewPosition, float4 fragPosLightSpace : FragmentPositionLightSpace, float3 fragmentNormal : Normal, float2 texCoord : TexCoord) : SV_TARGET
{
    //sample texture
    float4 textureColor = diffuseTex.Sample(smplr, texCoord);
    
    //normal
    float3 normal = normalize(fragmentNormal);
    
    //calculate light direction
    float3 lightDir = normalize(-lightDirection.xyz);
    
    //calculate diffuse light
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    float3 diffuse = diffuseStrength * lightColor;
    
    //calculate specular light
    float specularStrength = 0.5f;
    float3 viewDir = normalize(viewPos - fragPos);
    
    float3 r = dot(lightDir, normal) * normal * 2 - lightDir;
    
    float spec = pow(max(dot(viewDir, r), 0.0f), 32.0f);
    float3 specular = specularStrength * spec * lightColor;
    
    //ambient light
    float ambientStrength = 0.2f;
    float3 ambient = ambientStrength * lightColor;
    
    // calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    float3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    
    //float3 lighting = (ambient + diffuse + specular);
    
    return float4(lighting, 1.0f) * textureColor;
    //return float4(shadow.r, shadow.r, shadow.r, 1.0f); //shadow map debug
}