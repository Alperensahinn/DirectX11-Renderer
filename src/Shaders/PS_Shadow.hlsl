cbuffer lightBuffer : register(b1)
{
    float4 lightDirection;
    float4 lightColor;
    matrix lightSpaceMatrix;
};

Texture2D diffuseTex : register(t0);

SamplerState smplr;

float4 main(float3 fragPos : FragmentPosition, float3 viewPos : ViewPosition, float4 fragPosLightSpace : FragmentPositionLightSpace, float3 fragmentNormal : Normal, float2 texCoord : TexCoord) : SV_TARGET
{
    float4 f = fragPosLightSpace;
    return float4(f);
}