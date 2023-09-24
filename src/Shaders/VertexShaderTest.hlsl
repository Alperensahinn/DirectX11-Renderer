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
    float3x3 tbn : TBN;
    float2 texCoord : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float3 normal : Normal, float3 aTangent : Tangent, float3 aBitangent : Bitangent, float2 texCoord : TexCoord)
{
    VSOut vso;
    
    float4 f = mul(float4(pos, 1.0f), model);
    f = mul(f, lightSpaceMatrix);
    vso.fragPosLightSpace = f;
    
    vso.fragPos = mul(float4(pos, 1.0f), model).xyz;
    vso.viewPos = cameraPos.xyz;
    vso.pos = mul(float4(pos, 1.0f), modelViewProjection);
    vso.texCoord = texCoord;
    
    float3 T = normalize(mul(float4(aTangent, 0.0), model)).xyz;
    float3 B = normalize(mul(float4(aBitangent, 0.0), model)).xyz;
    float3 N = normalize(mul(float4(normal, 0.0), model)).xyz;
    float3x3 TBN = float3x3(T, B, N);
    vso.tbn = TBN;
    
    return vso;
}