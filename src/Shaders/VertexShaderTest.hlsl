cbuffer transformBuffer : register(b0)
{
    matrix transform;
};

struct VSOut
{
    float2 texCoord : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main( float3 pos : Position, float2 texCoord : TexCoord )
{
    VSOut vso;
    
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.texCoord = texCoord;
    
    return vso;
}