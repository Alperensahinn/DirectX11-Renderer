Texture2D diffuseTex : register(t0);

SamplerState smplr;

float4 main(float2 texCoord : TexCoord) : SV_TARGET
{
    float4 diffuse = diffuseTex.Sample(smplr, texCoord);
    return float4(diffuse);
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
}