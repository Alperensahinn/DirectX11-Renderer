cbuffer lightBuffer : register(b0)
{
    float4 lightDirection;
    float4 lightColor;
};

Texture2D diffuseTex : register(t0);

SamplerState smplr;

float4 main(float3 fragPos : FragmentPosition, float3 viewPos : ViewPosition, float3 fragmentNormal : Normal, float2 texCoord : TexCoord) : SV_TARGET
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
    
    return float4((specular + ambient + diffuse), 1.0f) * textureColor;
}