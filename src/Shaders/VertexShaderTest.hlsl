struct VSOut
{
    float2 texCoord : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main( float3 pos : Position, float2 texCoord : TexCoord )
{
    VSOut vso;
    
    vso.pos = float4(pos, 1.0f);
    vso.texCoord = texCoord;
    
    return vso;
}