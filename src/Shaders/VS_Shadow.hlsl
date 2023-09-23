cbuffer transformBuffer : register(b0)
{
    matrix modelViewProjection;
    matrix model;
    float4 cameraPos;
};

cbuffer lightBuffer : register(b1)
{
    float4 lightDirection;
    float4 lightColor;
    matrix lightSpaceMatrix;
};

struct VSOut
{
    float3 fragPos : FragmentPosition;
    float3 viewPos : ViewPosition;
    float4 fragPosLightSpace : FragmentPositionLightSpace;
    float3 fragmentNormal : Normal;
    float2 texCoord : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float3 normal : Normal, float2 texCoord : TexCoord)
{  
    VSOut vso;
    
    float4 f = mul(float4(pos, 1.0f), model);
    f = mul(f, lightSpaceMatrix);
    vso.pos = f;

    vso.fragPosLightSpace = f;
    
    vso.fragPos = mul(float4(pos, 1.0f), model).xyz;
    vso.viewPos = cameraPos.xyz;
    vso.fragmentNormal = normal;
    vso.texCoord = texCoord;
    
    return vso;
}