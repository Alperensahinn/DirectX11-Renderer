cbuffer transformBuffer : register(b0)
{
    matrix modelViewProjection;
    matrix model;
    float4 cameraPos;
};

cbuffer shadowBuffer : register(b1)
{
    matrix lightSpaceMatrix;
};

struct VSOut
{
    float3 fragPos : FragmentPosition;
    float3 viewPos : ViewPosition;
    float3 fragmentNormal : Normal;
    float2 texCoord : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float3 normal : Normal, float2 texCoord : TexCoord)
{  
    VSOut vso;
    
    float4 f = mul(float4(pos, 1.0f), model);
    vso.pos = mul(f, lightSpaceMatrix);

    vso.fragPos = mul(float4(pos, 1.0f), model).xyz;
    vso.viewPos = cameraPos.xyz;
    vso.fragmentNormal = normal;
    vso.texCoord = texCoord;
    
    return vso;
}