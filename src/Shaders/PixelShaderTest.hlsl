cbuffer lightBuffer : register(b0)
{
    float4 lightPosition;
    float4 lightColor;
};

Texture2D diffuseTex : register(t0);

SamplerState smplr;

float4 main(float4 fragPos : FragmentPosition, float4 viewPos : ViewPosition, float3 fragmentNormal : Normal, float2 texCoord : TexCoord) : SV_TARGET
{
    //sample texture
    float4 textureColor = diffuseTex.Sample(smplr, texCoord);
    
    //normal
    float3 normal = normalize(fragmentNormal);
    
    //calculate light direction
    float3 lightDir = normalize(lightPosition - fragPos);
    
    //calculate diffuse light
    float diffuseStrength = max(dot(lightDir, normal), 0.0f);
    float3 diffuse = diffuseStrength * lightColor;
    
    
    //calculate specular light
    float specularStrength = 0.5;
    
    float3 viewDir = normalize(viewPos - fragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float3 specular = specularStrength * spec * lightColor;
    
    
    //ambient light
    float ambientStrength = 0.2f;
    float3 ambient = ambientStrength * lightColor;
    
    
    return float4((diffuse + ambient + specular), 1.0f) * textureColor;
}